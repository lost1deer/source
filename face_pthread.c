#include <ucos_ii.h>
#include <face_pthread.h>
#include <face_errno.h>
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
    //    return  OSTaskCreateExt((void*)(*start_routine)((void *)arg),              /* Create the start task                                */
    //     (void          *) 0,
    //     (OS_STK        *) &AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
    //     (INT8U          ) APP_TASK_START_PRIO,
    //     (INT16U         ) *thread,
    //     (OS_STK        *) attr->stackaddr,
    //     (INT32U         ) attr->stacksize,
    //     (void          *) 0,
    //     (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
}