#include<face_time.h>
#include<face_signal.h>
int timer_create(clockid_t clockid, struct sigevent * evp, timer_t * timerid){
	timerid = clockid;
	if (evp == NULL){
		evp->sigev_notify = SIGEV_SIGNAL;
		/*�˴�Ҫ��sigev_signo����ΪĬ���ź����֣��ĵ�����ʱδ�ҵ���2022.08.03��*/
		evp->sigev_value.sival_int = timerid;
	}

	return 0;
}

int timer_delete(timer_t timerid){

}

/*������ɾ��restrict�ؼ���*/
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