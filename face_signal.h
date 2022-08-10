#include<signal.h>
#include<ucos_ii.h>
/*为sigev_notify的值定义以下符号常量，对于常量的值目前不清楚应该设定为多少（2022.08.03)*/
#define SIGEV_NONE    1u         /*发生相关事件不传递异步通知*/     
#define SIGEV_SIGNAL  2u         /*目标事件发生时生成具有程序定义值的排队信号*/
#define SIGEV_THREAD  3u         /*调用通知函数执行通知*/

union sigval{
	int sival_int;            /*整数型信号量*/
	void *sival_ptr;          /*指针型信号量*/
};

struct sigevent
{
	int sigev_notify;
	int sigev_signo;
	union sigval sigev_value;
	void (*sigev_notify_function)(union sigval);
	pthread_attr_t *sigev_notify_attributes;
};