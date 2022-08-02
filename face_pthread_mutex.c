#include <ucos_ii.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr){
    INT8U *perr;
    perr = (INT8U *)malloc(sizeof(INT8U));
    mutex = OSMutexCreate(*attr,perr);
}

int pthread_mutex_destroy(pthread_mutex_t *mutex){
    INT8U *perr;
    perr = (INT8U *)malloc(sizeof(INT8U));
    OSMutexDel(mutex,OS_DEL_ALWAYS,perr);
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    if (attr == NULL) {
        return 0;
    }

    memset(attr, 0, sizeof(pthread_mutexattr_t));

    return 1;
}