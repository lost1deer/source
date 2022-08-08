#include<face_time.h>
#include<face_signal.h>
int timer_create(clockid_t clockid, struct sigevent * evp, timer_t * timerid){
	timerid = clockid;
	if (evp == NULL){
		evp->sigev_notify = SIGEV_SIGNAL;
		/*此处要将sigev_signo定义为默认信号数字，文档中暂时未找到（2022.08.03）*/
		evp->sigev_value.sival_int = timerid;
	}

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