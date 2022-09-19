#include <ucos_ii.h>
#include <face_pthread.h>
#include <face_errno.h>
#include <face_pthread_time.h>
pthread_mutex_t g_pthread_lock = PTHREAD_MUTEX_INITIALIZER;
int pthread_create(pthread_t *thread,
       const pthread_attr_t * attr,
       void *(*start_routine)(void*), void * arg){
       
	   return OSTaskCreateExt((void(*)(void *))start_routine,              /* Create the start task                                */
        (void          *) 0,
        (OS_STK        *)((int *)attr->stackaddr+sizeof(CPU_STK)*(attr->stacksize-1)),
        (INT8U          ) attr->schedparam.sched_priority,
        (INT16U         ) *thread,
        (OS_STK        *) attr->stackaddr,
        (INT32U         ) attr->stacksize,
        (void          *) 0,
        (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
}
int pthread_detach(pthread_t thread)
{
    int ret = 0;
    pthread_tcb_t *ptcb = NULL;

    if (thread == NULL) {
        return EINVAL;
    }

    ptcb = __pthread_get_tcb(thread);
    if (ptcb == NULL) {
        return EINVAL;
    }

    if (ptcb->attr.detachstate == PTHREAD_CREATE_DETACHED) {
        return EINVAL;
    } else {
        ret = pthread_mutex_lock(&g_pthread_lock);
        if (ret != 0) {
            return EAGAIN;
        }
        ptcb->attr.detachstate = PTHREAD_CREATE_DETACHED;
        pthread_mutex_unlock(&g_pthread_lock);
    }

    return 0;
}
void pthread_cleanup_pop(int execute)
{
    pthread_tcb_t *ptcb = NULL;
    pthread_cleanup_t *cleanup = NULL;

    ptcb = __pthread_get_tcb(pthread_self());
    if (ptcb == NULL) {
        return;
    }

    cleanup = ptcb->cleanup;
    if (cleanup != NULL) {
        ptcb->cleanup = cleanup->prev;
        if (execute != 0) {
            cleanup->cleanup_routine(cleanup->para);
        }
        free(cleanup);
    }
}
void pthread_cleanup_push(void (*routine)(void *), void *arg)
{
    pthread_tcb_t *ptcb = NULL;
    pthread_cleanup_t *cleanup = NULL;

    ptcb = __pthread_get_tcb(pthread_self());
    if (ptcb == NULL) {
        return;
    }

    cleanup = (pthread_cleanup_t *) malloc(sizeof(pthread_cleanup_t));
    if (cleanup != NULL) {
        cleanup->cleanup_routine = routine;
        cleanup->para = arg;
        cleanup->prev = ptcb->cleanup;
        ptcb->cleanup = cleanup;
    }
}
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
{
    int ret = 0;

    ret = pthread_mutex_lock(&g_pthread_lock);
    if (ret != 0) {
        return EAGAIN;
    }

    if (*once_control == PTHREAD_ONCE_INIT)
    {
        *once_control = !PTHREAD_ONCE_INIT;

        pthread_mutex_unlock(&g_pthread_lock);

        init_routine ();
        return 0;
    }

    pthread_mutex_unlock(&g_pthread_lock);

    return 0;
}
int pthread_getcpuclockid(pthread_t thread, clockid_t *clock_id)
{
    if ((thread == NULL) || (clock_id == NULL)) {
        return EINVAL;
    }

    *clock_id = CLOCK_MONOTONIC;

    return 0;
}

int pthread_setname_np(pthread_t thread, const char *name)
{
    pthread_tcb_t *ptcb = NULL;

    if ((thread == NULL) || (name == NULL)) {
        return EINVAL;
    }

    ptcb = __pthread_get_tcb(thread);
    if (ptcb == NULL) {
        return EINVAL;
    }

    /* Truncate the name if it's too long. */
    OS_MemCopy(ptcb->thread_name, name, PTHREAD_NAME_MAX_LEN);
    if (OS_StrLen(name) > PTHREAD_NAME_MAX_LEN) {
        return ERANGE;
    }

    return 0;
}

int pthread_getname_np(pthread_t thread, char *name, size_t len)
{
    pthread_tcb_t *ptcb = NULL;

    if ((thread == NULL) || (name == NULL)) {
        return EINVAL;
    }

    if (len < PTHREAD_NAME_MAX_LEN) {
        return ERANGE;
    }

    ptcb = __pthread_get_tcb(thread);
    if (ptcb == NULL) {
        return EINVAL;
    }

    memset(name, 0, len);
    OS_MemCopy(name, ptcb->thread_name, len);

    return 0;
}

int pthread_getconcurrency(void)
{
    
    return 0;
}

int pthread_setconcurrency(int new_level)
{
    
    return ENOSYS;
}
int pthread_equal(pthread_t t1, pthread_t t2)
{
    return (int)(t1 == t2);
}
int pthread_cancel(pthread_t thread)
{
    return ENOSYS;
}

void pthread_testcancel(void)
{
    return;
}

int pthread_setcancelstate(int state, int *oldstate)
{
    return ENOSYS;
}

int pthread_setcanceltype(int type, int *oldtype)
{
    return ENOSYS;
}

int pthread_kill(pthread_t thread, int sig)
{
    return ENOSYS;
}

