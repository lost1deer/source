#define CLOCK_REALTIME           1u    /*ϵͳʵʱʱ��,��ϵͳʵʱʱ��ı���ı�,����UTC1970-1-1 0:0:0��ʼ��ʱ,�м�ʱ�����ϵͳʱ�䱻�û��ĳ�����,���Ӧ��ʱ����Ӧ�ı䡣*/
#define CLOCK_MONOTONIC          2u    /*��ϵͳ������һ����ʼ��ʱ,����ϵͳʱ�䱻�û��ı��Ӱ��*/
#define CLOCK_PROCESS_CPUTIME_ID 3u    /*�����̵���ǰ����ϵͳCPU���ѵ�ʱ�䡣��Ҫע���ǲ��ǽ��̿�ʼ����ǰ�����ʱ�䡣*/
#define CLOCK_THREAD_CPUTIME_ID  4u    /*���̵߳���ǰ����ϵͳCPU���ѵ�ʱ�䡣��Ҫע���ǲ����߳̿�ʼ����ǰ�����ʱ�䡣*/

#include<os_cpu.h>
#include<time.h>

typedef INT8U clockid_t;	/* ʱ�ӱ�ʶ				*/
// typedef INT32U clock_t;		/* ʱ����������			*/
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