#define _USE_32BIT_TIME_T

#include<os_cpu.h>
#include<time.h>

typedef INT8U clockid_t;	/* 时钟标识				*/
typedef INT32U clock_t;		/* 时钟跳数类型			*/
// typedef INT32U time_t;		/* 秒为单位的时间			*/
struct timespec {
	time_t  tv_sec;			/* 时间的秒数			*/		
	long    tv_nsec;		/* 时间的纳秒数			*/
};

int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *);
int clock_settime(clockid_t, const struct timespec *);
int nanosleep(const struct timespec *, struct timespec *);
unsigned int sleep(unsigned int seconds);