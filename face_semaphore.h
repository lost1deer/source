#ifndef   FACE_SEMAPHORE_H
#define   FACE_SEMAPHORE_H

#include "ucos_ii.h"
#include "face_time.h"

#define SEM_NSEMS_MAX   32767
#define SEM_VALUE_MAX   INT_MAX
#define SEM_FAILED ((sem_t *)0)
#define  O_CREAT 1u
#define  O_EXCL  2u
typedef int mode_t;


typedef struct {
    OS_EVENT *uos_sem;    // ljh: 这里修改成直接使用uc事件控制块的指针，只是把他封装成信号量
} sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);

sem_t *sem_open(const char *name, int oflag, ...);

sem_t *sem_open(const char *name, mode_t mode, unsigned int value);

int sem_post(sem_t *sem);

int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

int sem_trywait(sem_t *sem);

int sem_unlink(const char *name);

int sem_wait(sem_t *sem);

int sem_getvalue(sem_t *sem, int *sval);

int sem_close(sem_t *sem);

int sem_destroy(sem_t *sem);

#endif