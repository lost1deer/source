#include "ucos_ii.h"
#include "face_pthread.h"
#include "face_sched.h"
#include "face_errno.h"

static CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];
typedef INT32U size_t;

int pthread_attr_init(pthread_attr_t *attr) {
    struct sched_param param;
    param.sched_priority = APP_TASK_START_PRIO;
    param.slice = PTHREAD_DEFAULT_SLICE;
    //memset(attr, 0, sizeof(pthread_attr_t));
    attr->flag = PTHREAD_DYN_INIT;
    attr->stacksize = APP_TASK_START_STK_SIZE;
    attr->schedparam = param;
    attr->detachstate = PTHREAD_CREATE_JOINABLE;
    attr->inheritsched = PTHREAD_EXPLICIT_SCHED;
    attr->guardsize = PTHREAD_DEFAULT_GUARD_SIZE;
    attr->stackaddr = &AppTaskStartStk[0];
    attr->scope = PTHREAD_SCOPE_SYSTEM;
    return 0;    // ljh: 不知道为啥这里return 1，改成0才对
}

int pthread_attr_destroy(pthread_attr_t *attr) {
    if (attr == NULL) {
        return pthread_flase;
    }

    memset(attr, 0, sizeof(pthread_attr_t));

    return 1;
}

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate) {

    if ((attr == NULL) || ((detachstate != PTHREAD_CREATE_DETACHED) &&
                           (detachstate != PTHREAD_CREATE_JOINABLE))) {
        return pthread_flase;
    }

    attr->detachstate = detachstate;

    return 1;
}

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate) {
    if ((attr == NULL) || (detachstate == NULL)) {
        return pthread_flase;
    }

    *detachstate = attr->detachstate;

    return 0;
}

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy) {
    if ((attr == NULL) || ((policy < SCHED_OTHER) || (policy > SCHED_RR))) {
        return pthread_flase;
    }

    attr->schedpolicy = policy;

    return 0;
}

int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy) {
    if ((attr == NULL) || (policy == NULL)) {
        return pthread_flase;
    }

    *policy = attr->schedpolicy;

    return 0;
}

int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param) {
    if ((attr == NULL) || (param == NULL)) {
        return pthread_flase;
    }

    attr->schedparam.sched_priority = param->sched_priority;
    attr->schedparam.slice = param->slice;
    return 0;
}

int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param) {
    if ((attr == NULL) || (param == NULL)) {
        return pthread_flase;
    }

    param->sched_priority = attr->schedparam.sched_priority;
    param->slice = attr->schedparam.slice;

    return 0;
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize) {
    if ((attr == NULL) || (stacksize <= 0)) {
        return pthread_flase;
    }

    attr->stacksize = stacksize;

    return 0;
}

int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize) {
    if ((attr == NULL) || (stacksize == NULL)) {
        return pthread_flase;
    }

    *stacksize = attr->stacksize;

    return 0;
}

int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr) {
    if ((attr == NULL) || (stackaddr == NULL)) {
        return pthread_flase;
    }

    attr->stackaddr = stackaddr;

    return 0;
}

int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr) {
    if ((attr == NULL) || (stackaddr == NULL)) {
        return pthread_flase;
    }

    *stackaddr = attr->stackaddr;

    return 0;
}

int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize) {
    if ((attr == NULL) || (stackaddr == NULL) || (stacksize <= 0)) {
        return pthread_flase;
    }

    attr->stackaddr = stackaddr;
    attr->stacksize = stacksize;

    return 0;
}

int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize) {
    if ((attr == NULL) || (stackaddr == NULL) || (stacksize == NULL)) {
        return pthread_flase;
    }

    *stackaddr = attr->stackaddr;
    *stacksize = attr->stacksize;

    return 0;
}

int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched) {
    if ((attr == NULL) || (inheritsched == NULL)) {
        return pthread_flase;
    }

    *inheritsched = attr->inheritsched;

    return 0;
}

int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched) {
    if ((attr == NULL) || (inheritsched < PTHREAD_INHERIT_SCHED) ||
        (inheritsched > PTHREAD_EXPLICIT_SCHED)) {
        return pthread_flase;
    }

    attr->inheritsched = inheritsched;

    return 0;
}

int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize) {
    /* guardsize stack protection is not supported by kernel */
    return ENOSYS;
}

int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize) {
    /* guardsize stack protection is not supported by kernel */
    return ENOSYS;
}

int pthread_attr_getscope(const pthread_attr_t *attr, int *contentionscope) {
    if ((attr == NULL) || (contentionscope == NULL)) {
        return pthread_flase;
    }

    *contentionscope = attr->scope;

    return 0;
}

int pthread_attr_setscope(pthread_attr_t *attr, int contentionscope) {
    if ((attr == NULL) ||
        ((contentionscope != PTHREAD_SCOPE_PROCESS) && (contentionscope != PTHREAD_SCOPE_SYSTEM))) {
        return pthread_flase;
    }

    attr->scope = contentionscope;

    return 0;
}