#include "ucos_ii.h"
#include "face_time.h"
#include "face_sched.h"

#ifndef   FACE_PTHREAD_H
#define   FACE_PTHREAD_H

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
#define uos_WAIT_FOREVER 0xffffffffu /**< 阻塞性等待，即一直等待，直到事件发生或资源获得才返回 */
#define uos_NO_WAIT      0x0         /**< 非阻塞性等待，即若无事件发生或无资源可获得，则返回 */

typedef void * uos_hdl_t;
typedef uos_hdl_t uos_mutex_t;
typedef INT32U size_t;
typedef INT16U pthread_t;
typedef int pthread_once_t;
//typedef struct os_event pthread_mutex_t;
//typedef unsigned int pthread_mutexattr_t;
typedef unsigned char uint8_t;

typedef struct pthread_attr_t
{
	uint8_t flag;
    int        detachstate;         //线程的分离状态
    int        schedpolicy;         //线程调度策略
	struct sched_param schedparam;  //线程调度参数
    int         inheritsched;        //线程的继承性
    int         scope;         //线程的作用域
    long unsigned int      guardsize;         //线程栈末尾的警戒缓冲区大小
    int         stackaddr_set;
    void *      stackaddr;         //线程栈的位置
	long unsigned int      stacksize;         //线程栈的大小
} pthread_attr_t;

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

typedef struct pthread_cleanup {
    int cancel_type;
    struct pthread_cleanup *prev;
    void (*cleanup_routine)(void *para);
    void *para;
} pthread_cleanup_t;

typedef struct pthread_tcb {
    void *task;    /* The uc task handle. */
    unsigned int magic; /* The pthread tcb memory magic number. */
    void *(*thread_entry)(void *para); /* The start routine of the thread. */
    void *thread_para;   /* The parameter of start routine. */
    void *join_sem;  /* The semaphore for pthread_join. */
    pthread_cleanup_t *cleanup; /* The registered cleanup function for the thread.*/
    void *environ;
    void **tls;
    void  *return_value; /* The thread's return value. */
    pthread_attr_t  attr; /* The thread's attribute. */
    char thread_name[PTHREAD_NAME_MAX_LEN + 1];  /* The thread's name. */
} pthread_tcb_t;

pthread_tcb_t* __pthread_get_tcb(pthread_t thread);

int       pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                         void *(*start_routine)(void *), void *arg);
int       pthread_detach(pthread_t thread);
int       pthread_cancel(pthread_t thread);
void      pthread_testcancel(void);

pthread_t pthread_self(void); // need to support

int       pthread_setcancelstate(int state, int *oldstate);
int       pthread_setcanceltype(int type, int *oldtype);
int       pthread_kill(pthread_t thread, int sig);
int       pthread_equal(pthread_t t1, pthread_t t2);
void      pthread_cleanup_pop(int execute);
void      pthread_cleanup_push(void (*routine)(void *), void *arg);
int       pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
int       pthread_getcpuclockid(pthread_t thread_id, clockid_t *clock_id);
int       pthread_setconcurrency(int new_level);
int       pthread_getconcurrency(void);
int       pthread_setname_np(pthread_t thread, const char *name);
int       pthread_getname_np(pthread_t thread, char *name, size_t len);

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

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *at);
int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling);

#endif