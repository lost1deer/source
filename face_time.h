#define _USE_32BIT_TIME_T

#include<os_cpu.h>
#include<time.h>

typedef INT8U clockid_t;	/* ʱ�ӱ�ʶ				*/
typedef INT32U clock_t;		/* ʱ����������			*/
// typedef INT32U time_t;		/* ��Ϊ��λ��ʱ��			*/
struct timespec {
	time_t  tv_sec;			/* ʱ�������			*/		
	long    tv_nsec;		/* ʱ���������			*/
};

int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *);
int clock_settime(clockid_t, const struct timespec *);
int nanosleep(const struct timespec *, struct timespec *);
unsigned int sleep(unsigned int seconds);