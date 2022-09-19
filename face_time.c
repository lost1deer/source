#include "face_time.h"
#include "ucos_ii.h"
#include "face_errno.h"

int errno = 0;

static long long start_time_ms = 0;

clock_t ms_to_ticks(sys_time_t ms)        /* 从毫秒时间转为时钟节拍 */
{
	INT16U padding;
	clock_t ticks;

	padding = 1000 / OS_TICKS_PER_SEC;
	padding = (padding > 0) ? (padding - 1) : 0;

	ticks = ((ms + padding) * OS_TICKS_PER_SEC) / 1000;

	return ticks;
}

sys_time_t ticks_to_ms(clock_t ticks)    /* 从时钟节拍转为毫秒时间 */
{
	INT32U padding;
	sys_time_t time;

	padding = OS_TICKS_PER_SEC / 1000;
	padding = (padding > 0) ? (padding - 1) : 0;

	time = ((ticks + padding) * 1000) / OS_TICKS_PER_SEC;

	return time;
}

sys_time_t sys_time_get(void)           /* 获取系统时间的毫秒表示 */
{
	return (sys_time_t)(OSTimeGet() * 1000 / OS_TICKS_PER_SEC);
}

sys_time_t real_time_get(void)          /* 获取系统实时时间 */
{
	return sys_time_get() + start_time_ms;
}

void real_time_set(sys_time_t now_ms) {
	start_time_ms = now_ms - sys_time_get();
}

int clock_getres(clockid_t clock_id, struct timespec *res)
{
	if (((clock_id != CLOCK_REALTIME) && (clock_id != CLOCK_MONOTONIC)) || (res == NULL)) {
		errno = EINVAL;    // 不懂errno是在哪定义的？
		return -1;
	}

	res->tv_sec = 0;
	res->tv_nsec = 1e6;    // 系统时钟分辨率设为毫秒级

	return 0;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
	sys_time_t time_ms = 0;
	
	if (tp == NULL) {
		errno = EINVAL;
		return -1;
	}

	if (clock_id == CLOCK_MONOTONIC) {
		time_ms = sys_time_get();
	} else if (clock_id == CLOCK_REALTIME) {
		time_ms = real_time_get();
	}
	else {
		errno = EINVAL;
		return -1;
	}
	tp->tv_sec = time_ms / 1000;
	tp->tv_nsec = (time_ms % 1000) * 1000000;
	return 0;
}

int clock_settime(clockid_t clock_id, const struct timespec *tp)
{
	sys_time_t time_ms = 0;
	/* 只能设置实时时间 */
	if ((clock_id != CLOCK_REALTIME) || (tp == NULL) ||
		(tp->tv_nsec < 0) || (tp->tv_nsec >= 1000000000UL)) {
		errno = EINVAL;
		return -1;
	}

	time_ms = (tp->tv_sec * 1000) + (tp->tv_nsec / 1000000);
	real_time_set(time_ms);

	return 0;
}

int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp)
{

}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
	sys_time_t time_ms = 0;
	if ((rqtp == NULL) || (rqtp->tv_sec < 0) || (rqtp->tv_nsec < 0)
		|| (rqtp->tv_nsec >= 1000000000UL)) {
		errno = EINVAL;
		return -1;
	}

	time_ms = (rqtp->tv_sec * 1000) + (rqtp->tv_nsec / 1000000);
	OSTimeDlyHMSM(0, 0, 0, time_ms);

	return 0;
}

unsigned int sleep(unsigned int seconds)
{
	return OSTimeDlyHMSM(0, 0, seconds, 0);
}

INT32U timespce2ticks(const struct timespec *time_) {
	sys_time_t time_ms = (time_->tv_sec * 1000) + (time_->tv_nsec / 1000000);
	return (INT32U) ms_to_ticks(time_ms);
}