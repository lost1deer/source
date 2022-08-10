#define CLOCK_REALTIME           0    /* ϵͳʵʱʱ��,��ϵͳʵʱʱ��ı���ı�,����UTC1970-1-1 0:0:0��ʼ��ʱ,�м�ʱ�����ϵͳʱ�䱻�û��ĳ�����,���Ӧ��ʱ����Ӧ�ı�. */
#define CLOCK_MONOTONIC          1    /* ��ϵͳ������һ����ʼ��ʱ,����ϵͳʱ�䱻�û��ı��Ӱ�� */
#define CLOCK_PROCESS_CPUTIME_ID 2    /* �����̵���ǰ����ϵͳCPU���ѵ�ʱ�䡣��Ҫע���ǲ��ǽ��̿�ʼ����ǰ�����ʱ��. */
#define CLOCK_THREAD_CPUTIME_ID  3    /* ���̵߳���ǰ����ϵͳCPU���ѵ�ʱ�䡣��Ҫע���ǲ����߳̿�ʼ����ǰ�����ʱ��. */

#include<os_cpu.h>
#include<time.h>

typedef INT8U clockid_t;    /* ʱ�ӱ�ʶ              */
typedef INT32U clock_t;     /* ʱ����������          */
#ifndef _TIME_T_DEFINED
typedef INT32U time_t;      /* ��Ϊ��λ��ʱ��        */
#endif
typedef long long sys_time_t;  /* ����Ϊ��λ��ʱ��      */
typedef INT32U timer_t;     /* �����洢��ʱ��ID      */


struct timespec {
	time_t  tv_sec;         /* ʱ�������            */		
	long    tv_nsec;        /* ʱ���������          */
};

#ifndef _TM_DEFINED
struct tm {
	int tm_sec;     /* seconds after the minute - [0,59] */
	int tm_min;     /* minutes after the hour - [0,59] */
	int tm_hour;    /* hours since midnight - [0,23] */
	int tm_mday;    /* day of the month - [1,31] */
	int tm_mon;     /* months since January - [0,11] */
	int tm_year;    /* years since 1900 */
	int tm_wday;    /* days since Sunday - [0,6] */
	int tm_yday;    /* days since January 1 - [0,365] */
	int tm_isdst;   /* daylight savings time flag */
};
#define _TM_DEFINED
#endif  /* _TM_DEFINED */

struct itimerspec{
	struct timespec it_interval;
	struct timespec it_value;
};


int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_settime(clockid_t, const struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *);
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