#include<face_time.h>
#include<ucos_ii.h>

int clock_getres(clockid_t clock_id, struct timespec *res) {
	res->tv_sec = 0;
	res->tv_nsec = 1 / OS_TICKS_PER_SEC;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp) {
	clock_t ticks = OSTimeGet();
	tp->tv_sec = ((INT32U)ticks) / OS_TICKS_PER_SEC;
	tp->tv_nsec = tp->tv_sec / 1e9;
}

int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp) {

}


int clock_settime(clockid_t clock_id, const struct timespec *tp) {
	clock_t ticks = tp->tv_nsec * 1000 * OS_TICKS_PER_SEC;
	OSTimeSet((INT32U)ticks);
}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp) {

}

unsigned int sleep(unsigned int seconds)
{
	return OSTimeDlyHMSM(0, 0, seconds, 0);
}