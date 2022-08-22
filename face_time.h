#ifndef   FACE_TIME_H
#define   FACE_TIME_H
#define CLOCK_REALTIME           0    /* 系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,中间时刻如果系统时间被用户改成其他,则对应的时间相应改变. */
#define CLOCK_MONOTONIC          1    /* 从系统启动这一刻起开始计时,不受系统时间被用户改变的影响 */
#define CLOCK_PROCESS_CPUTIME_ID 2    /* 本进程到当前代码系统CPU花费的时间。需要注意是不是进程开始到当前代码的时间. */
#define CLOCK_THREAD_CPUTIME_ID  3    /* 本线程到当前代码系统CPU花费的时间。需要注意是不是线程开始到当前代码的时间. */

#include<os_cpu.h>
#include<face_signal.h>
typedef INT8U clockid_t;    /* 时钟标识              */
typedef INT32U clock_t;     /* 时钟跳数类型          */
#ifndef _TIME_T_DEFINED
typedef INT32U time_t;      /* 秒为单位的时间        */
#endif
typedef long long sys_time_t;  /* 毫秒为单位的时间      */
typedef INT32U timer_t;     /* 用来存储计时器ID      */

/*时钟列表*/
typedef struct timer_list_s {
	timer_t   id;
	void * uos_timer;
	void *evp;  /* The sigevent as the parameter of timer_callback. */
	struct timer_list_s *next;
} timer_list_t;

struct timespec {
	time_t  tv_sec;         /* 时间的秒数            */		
	long    tv_nsec;        /* 时间的纳秒数          */
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

/*---计时器---*/
int timer_create(clockid_t clockid, struct sigevent *evp, timer_t * timerid);
int timer_delete(timer_t timerid);
int timer_settime(timer_t timerid, int flags,
	const struct itimerspec *value,
struct itimerspec *ovalue);  /*这里曾删除restrict关键字（2022.08.02）*/
int timer_gettime(timer_t timerid, struct itimerspec *value);
int timer_getoverrun(timer_t timerid);


INT32U timespce2ticks(const struct timespec*);
#endif