#include<face_time.h>
#include<face_signal.h>
#include<face_errno.h>
#include <face_pthread.h>
#include<ucos_ii.h>
#include<stdlib.h>
#include <string.h>
#define POSIX_TIMER_ID_MIN 1
void * callback_arg;
timer_list_t *g_timer_list_head = NULL; /* The timer list head. */
pthread_mutex_t g_timer_lock = PTHREAD_MUTEX_INITIALIZER;  /* The lock to protect timer list. */

static void timer_callback(void *timer, void *arg){
	struct sigevent *evp = (struct sigevent *)arg;

	if ((evp != NULL) && (evp->sigev_notify_function != NULL)) {
		evp->sigev_notify_function(evp->sigev_value);
	}
}

static int64_t timespec_to_nanosecond(struct timespec *t){
	return ((uint64_t)t->tv_sec * 1000000000UL + t->tv_nsec);
}

static struct timespec nanosecond_to_timespec(int64_t ns)
{
	struct timespec ret_time;

	ret_time.tv_sec = ns / 1000000000UL;
	ret_time.tv_nsec = ns % 1000000000UL;

	return ret_time;
}
static int timespec_abs_to_relate(struct timespec *time_abs, struct timespec *time_relate)
{
	int ret = 0;
	int64_t ns = 0;
	struct timespec time_now;

	memset(&time_now, 0, sizeof(time_now));

	ret = clock_gettime(CLOCK_MONOTONIC, &time_now);
	if (ret != 0) {
		return -1;
	}

	ns = timespec_to_nanosecond(time_abs) - timespec_to_nanosecond(&time_now);
	if (ns < 0) {
		return -1;
	}

	*time_relate = nanosecond_to_timespec(ns);

	return 0;
}


int timer_create(clockid_t clockid, struct sigevent * evp, timer_t * timerid){
	INT8U *perr = NULL;
	int ret = 0;
	timer_list_t *timer_list_m = NULL;
	timer_list_t *timer_list = NULL;

	if ((evp == NULL) || (timerid == NULL) ||
		((clockid != CLOCK_REALTIME) && (clockid != CLOCK_MONOTONIC))) {
		errno = EINVAL;
		return -1;
	}

	/* Only support SIGEV_THREAD. */
	if (evp->sigev_notify != SIGEV_THREAD) {
		errno = ENOTSUP;
		return -1;
	}

	/* The sigev_notify_function should be set for SIGEV_THREAD */
	if (evp->sigev_notify_function == NULL) {
		errno = EINVAL;
		return -1;
	}

	/* 对计时器分配相应空间 */
	timer_list_m = (timer_list_t *)malloc(sizeof(timer_list_t));
	if (timer_list_m == NULL) {
		return -1;
	}

	memset(timer_list_m, 0, sizeof(timer_list_t));

	timer_list_m->evp = malloc(sizeof(struct sigevent));
	if (timer_list_m->evp == NULL) {
		free(timer_list_m);
		timer_list_m = NULL;
		return -1;
	}
	memcpy(timer_list_m->evp, evp, sizeof(struct sigevent));

	ret = pthread_mutex_lock(&g_timer_lock);
	if (ret != 0) {
		free(timer_list_m->evp);
		free(timer_list_m);
		return -1;
	}

	/* find the last node add the new timer to the list */
	if (g_timer_list_head == NULL) {
		/* 初始化一个id */
		timer_list_m->id = (timer_t)POSIX_TIMER_ID_MIN;
		g_timer_list_head = timer_list_m;
	}
	else {
		timer_list = g_timer_list_head;
		while (timer_list->next != NULL) {
			timer_list = timer_list->next;
		}

		/* 新计时器id直接+1 */
		timer_list_m->id = (int)timer_list->id + 1;
		timer_list->next = timer_list_m;
	}

	pthread_mutex_unlock(&g_timer_lock);

	/* 创建计时器 */
	timer_list_m->uos_timer = OSTmrCreate(0, 0, OS_TMR_OPT_ONE_SHOT, 
		timer_callback, &callback_arg, &(timer_list_m->id), &perr);

	/* update the timerid */
	*timerid = timer_list_m->id;

	return 0;
}

int timer_delete(timer_t timerid){
	INT8U *perr = NULL;
	timer_list_t *timer_list = NULL;
	timer_list_t *timer_list_l = NULL;
	int ret = 0;

	/* Timer list is empty. */
	if (g_timer_list_head == NULL) {
		return -1;
	}

	ret = pthread_mutex_lock(&g_timer_lock);
	if (ret != 0) {
		return -1;
	}

	/* Scan the list to find the timer according to timerid. */
	timer_list = g_timer_list_head;
	timer_list_l = g_timer_list_head;
	while ((timer_list != NULL) && (timer_list->id != timerid)) {
		timer_list_l = timer_list;
		timer_list = timer_list->next;
	}

	/* Do not find the timerid. */
	if (timer_list == NULL) {
		pthread_mutex_unlock(&g_timer_lock);
		return -1;
	}

	/* Stop and detete the timer. */
	OSTmrStop(&(timer_list->uos_timer), OS_TMR_OPT_NONE, callback_arg, &perr);
	OSTmrDel(&(timer_list->uos_timer), &perr);
	/* Delete the timer from the list and free the timer.*/
	if (timer_list_l == g_timer_list_head) {
		g_timer_list_head = timer_list->next;
	}
	else {
		timer_list_l->next = timer_list->next;
	}
	if (timer_list->evp != NULL) {
		free(timer_list->evp);
	}
	free(timer_list);
	pthread_mutex_unlock(&g_timer_lock);
	return ret;
}

/*这里曾删除restrict关键字*/
int timer_settime(timer_t timerid, int flags,
	const struct itimerspec * value,
    struct itimerspec *ovalue){  
	INT8U *perr = NULL;
	int     ret = 0;
	int64_t value_ns = 0;
	int64_t interval_ns = 0;
	struct timespec value_spec = { 0 };
	struct timespec interval_spec = { 0 };
	timer_list_t   *timer_list = NULL;

	if (value == NULL) {
		return -1;
	}

	if (g_timer_list_head == NULL) {
		return -1;
	}

	/* If the time is absolute time transform it to relative time. */
	if ((flags & TIMER_ABSTIME) == TIMER_ABSTIME) {
		ret = timespec_abs_to_relate((struct timespec *)&value->it_value, &value_spec);
		ret |= timespec_abs_to_relate((struct timespec *)&value->it_interval, &interval_spec);
		if (ret != 0) {
			return -1;
		}

		value_ns = timespec_to_nanosecond(&value_spec);
		interval_ns = timespec_to_nanosecond(&interval_spec);
	}
	else {
		value_ns = timespec_to_nanosecond((struct timespec *)&value->it_value);
		interval_ns = timespec_to_nanosecond((struct timespec *)&value->it_interval);
	}

	/* Get the old parameters of timer if ovalue is not NULL. */
	if (ovalue != NULL) {
		ret = timer_gettime(timerid, ovalue);
		if (ret != 0) {
			return -1;
		}
	}

	ret = pthread_mutex_lock(&g_timer_lock);
	if (ret != 0) {
		return -1;
	}

	/* Scan the list to find the timer according to timerid. */
	timer_list = g_timer_list_head;
	while ((timer_list != NULL) && (timer_list->id != timerid)) {
		timer_list = timer_list->next;
	}

	/* Do not find the timer. */
	if (timer_list == NULL) {
		pthread_mutex_unlock(&g_timer_lock);
		return -1;
	}

	OSTmrStop(&(timer_list->uos_timer), OS_TMR_OPT_NONE, callback_arg, &perr);
	/* uos_timer_change? */
	OSTmrStart(&(timer_list->uos_timer), &perr);

	pthread_mutex_unlock(&g_timer_lock);
	return ret;

}

int timer_gettime(timer_t timerid, struct itimerspec *value)
{
	int ret = 0;
	uint64_t time_ns[4] = { 0 };
	timer_list_t *timer_list = NULL;

	if (value == NULL) {
		return -1;
	}

	ret = pthread_mutex_lock(&g_timer_lock);
	if (ret != 0) {
		return -1;
	}

	/* Scan the list to find the timer according to timerid */
	timer_list = g_timer_list_head;
	while (timer_list != NULL) {
		if (timer_list->id == timerid) {
			break;
		}
		timer_list = timer_list->next;
	}

	if (timer_list == NULL) {
		/* The timerid is not found. */
		pthread_mutex_unlock(&g_timer_lock);
		return -1;
	}
	/* interval = period; value = match */
	value->it_interval.tv_sec = timer_list->uos_timer->OSTmrPeriod / 1000;
	value->it_interval.tv_nsec = (timer_list->uos_timer->OSTmrPeriod % 1000) * 1000000UL;
	value->it_value.tv_sec = timer_list->uos_timer->OSTmrMatch / 1000;
	value->it_value.tv_nsec = (timer_list->uos_timer->OSTmrMatch % 1000) * 1000000UL;

	pthread_mutex_unlock(&g_timer_lock);
	return 0;
}

int timer_getoverrun(timer_t timerid)
{
	errno = ENOSYS;
	return -1;
}