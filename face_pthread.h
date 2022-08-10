#include<ucos_ii.h>
#define PTHREAD_DEFAULT_STACK_SIZE 2048
#define PTHREAD_DEFAULT_GUARD_SIZE 256
#define PTHREAD_DEFAULT_PRIORITY   30
#define PTHREAD_DEFAULT_SLICE      10
#define PTHREAD_NAME_MAX_LEN  16

#define PTHREAD_TCB_MAGIC 0X11223344

#define DEFAULT_MUTEX_TYPE        PTHREAD_MUTEX_DEFAULT
#define DEFAULT_MUTEX_PROCOCOL    PTHREAD_PRIO_INHERIT
#define DEFAULT_MUTEX_PRIOCEILING 30
#define DEFAULT_MUTEX_PSHARED     PTHREAD_PROCESS_PRIVATE

#define DEFAULT_COND_CLOCK  CLOCK_REALTIME
#define DEFAULT_COND_SHARED PTHREAD_PROCESS_PRIVATE
/* Defined for API with delay time */
#define AOS_WAIT_FOREVER 0xffffffffu /**< 阻塞性等待，即一直等待，直到事件发生或资源获得才返回 */
#define AOS_NO_WAIT      0x0         /**< 非阻塞性等待，即若无事件发生或无资源可获得，则返回 */
typedef void * aos_hdl_t;
typedef aos_hdl_t aos_mutex_t;
typedef INT32U size_t;
typedef INT16U pthread_t;
typedef struct os_event pthread_mutex_t;
typedef unsigned int pthread_mutexattr_t;
typedef unsigned char uint8_t;
typedef struct
{
       int        detachstate;         //线程的分离状态
       int        schedpolicy;         //线程调度策略
       struct sched_param        schedparam;        //线程的调度参数
       int         inheritsched;        //线程的继承性
       int         scope;         //线程的作用域
       long unsigned int      guardsize;         //线程栈末尾的警戒缓冲区大小
       int         stackaddr_set;
       void *      stackaddr;         //线程栈的位置
	   long unsigned int      stacksize;         //线程栈的大小
}pthread_attr_t;
typedef struct pthread_mutexattr {
    uint8_t flag;
    int type;
    int protocol;
    int prioceiling;
    int pshared;
    INT8U   prio;
    INT8U  *perr;
} pthread_mutexattr_t;

typedef struct pthread_mutex {
    uint8_t              flag;
    OS_EVENT                *mutex;
    pthread_mutexattr_t  attr;
} pthread_mutex_t;
int pthread_attr_init(pthread_attr_t *attr);
int pthread_create(pthread_t * thread,
      const pthread_attr_t * attr,
      void *(*start_routine)(void*), void * arg);


int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getschedpolicy(pthread_attr_t const *attr, int *policy);
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);
int pthread_attr_getinheritsched(const pthread_attr_t * attr, int * inheritsched);
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
int pthread_attr_setscope(pthread_attr_t *attr, int contentionscope);
int pthread_attr_getscope(const pthread_attr_t *attr, int *contentionscope);