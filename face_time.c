#include<face_time.h>
#include<ucos_ii.h>

int clock_getres(clockid_t clock_id, struct timespec *res) {
	clock_t ticks = OSTimeGet();
	time_t time_in_second = ((INT32U) ticks) / OS_TICKS_PER_SEC;
	res->tv_sec = time_in_second;


	return 0;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp) {

}

int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp) {

}


int clock_settime(clockid_t clock_id, const struct timespec *tp) {

}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp) {

}

unsigned int sleep(unsigned int seconds)
{
	INT32U ticks;


	if (OSIntNesting > 0u) {                     /* See if trying to call from an ISR                  */
		return (OS_ERR_TIME_DLY_ISR);
	}
	if (OSLockNesting > 0u) {                    /* See if called with scheduler locked                */
		return (OS_ERR_SCHED_LOCKED);
	}
#if OS_ARG_CHK_EN > 0u
	if (seconds == 0u){
		return (OS_ERR_TIME_ZERO_DLY);
	}
	if (seconds> 3540u){                         /*3540=59*60                                          */
		return (OS_ERR_TIME_INVALID_MINUTES);    /* Validate arguments to be within range              */
	}
	if (seconds > 59u) {
		return (OS_ERR_TIME_INVALID_SECONDS);
	}
#endif
	/* Compute the total number of clock ticks required.. */
	/* .. (rounded to the nearest tick)                   */
	ticks = ((INT32U)seconds) * OS_TICKS_PER_SEC;
	OSTimeDly(ticks);
	return (OS_ERR_NONE);
}