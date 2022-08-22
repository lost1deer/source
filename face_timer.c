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
		timer_callback, callback_arg, &(timer_list_m->id), perr);

	/* update the timerid */
	*timerid = timer_list_m->id;

	return 0;
}

int timer_delete(timer_t timerid){

}

/*这里曾删除restrict关键字*/
int timer_settime(timer_t timerid, int flags,
	const struct itimerspec * value,
    struct itimerspec *ovalue){      


}

int timer_gettime(timer_t timerid, struct itimerspec *value)
{

}

int timer_getoverrun(timer_t timerid)
{

}