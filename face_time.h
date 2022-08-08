#define _USE_32BIT_TIME_T

#include<os_cpu.h>
#include<time.h>

typedef INT8U clockid_t;	/* ʱ�ӱ�ʶ				*/
typedef INT32U clock_t;		/* ʱ����������			*/
// typedef INT32U time_t;		/* ��Ϊ��λ��ʱ��			*/
typedef INT32U timer_t;      /* �����洢��ʱ��ID      */


struct timespec {
	time_t  tv_sec;			/* ʱ�������			*/		
	long    tv_nsec;		/* ʱ���������			*/
};



struct itimerspec{
	struct timespec it_interval;
	struct timespec it_value;
};


int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *);
int clock_settime(clockid_t, const struct timespec *);
int nanosleep(const struct timespec *, struct timespec *);
unsigned int sleep(unsigned int seconds);

/*---��ʱ��---*/
int timer_create(clockid_t clockid, struct sigevent *evp, timer_t * timerid);
int timer_delete(timer_t timerid);
int timer_settime(timer_t timerid, int flags,
	const struct itimerspec *value,
struct itimerspec *ovalue);  /*������ɾ��restrict�ؼ��֣�2022.08.02��*/
int timer_gettime(timer_t timerid, struct itimerspec *value);
int timer_getoverrun(timer_t timerid);