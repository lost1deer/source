#include"face_semaphore.h"
#include "ucos_ii.h"
#include"face_errno.h"
#include<stdlib.h>
#include"face_time.h"

#define SEM_NAME_MAX 1024

OS_MEM *SemBuffer;
INT8U SemMemory[16][4];    // ljh: 划分16个内存块，每块有4个字节，用来动态创建 sem_t

OS_MEM *SemDataBuffer;
INT8U SemDataMemory[16][64];    // ljh: 划分16个内存块，每块有64个字节（不知道够不够），用来创建 OS_SEM_DATA

INT8U first_time = 1;    // ljh: 第一次初始化时创建内存分区

int sem_init(sem_t *sem, int pshared, unsigned int value) {
    if (first_time) {
        INT8U perr;
        SemBuffer = OSMemCreate(SemMemory, 16, 4, &perr);    // ljh: 简单写一下，后续要判断是否分配成功
        SemDataBuffer = OSMemCreate(SemDataMemory, 16, 64, &perr);
        first_time = 0;
    }

    if ((sem == NULL) || value > SEM_VALUE_MAX) {
        errno = EINVAL;
        return -1;
    }
    if ((value == 0) || value > SEM_NSEMS_MAX) {
        errno = ENOSPC;
        return -1;
    }
    /* Sharing between processes is not supported. (注：该部分在FACE文档中并未提到)*/
    if (pshared != 0) {
        errno = ENOTSUP;
        return -1;
    }
    /*未完成：
     *     1. 该进程缺少初始化信号量的适当权限。（2022.08.08）
     *     2. 关于pshared变量而产生的操作，涉及到自旋锁（spin locks）
     *     3. 初始化信号量已达信号量限制？
     */
    sem->uos_sem = OSSemCreate(value);
    return 0;
}

sem_t *sem_open(const char *name, int oflag, mode_t mode, int value) {
    sem_t *new_sem;
    INT8U perr;    // ljh: 这里需要一个perr变量，而不是空指针，下面相应的变成&
    int *pname = *name;
    if (strlen(name) <= 0) {
        errno = ENOENT;
        return -1;
    }

    if (strlen(name) > SEM_NAME_MAX) {
        errno = ENAMETOOLONG;
        return -1;
    }
    if (oflag = O_CREAT) {
        new_sem = OSMemGet(SemBuffer, &perr);    // ljh: 动态分配内存
        new_sem->uos_sem = OSSemCreate(value);
        OSEventNameSet(new_sem->uos_sem, pname, &perr);
        return new_sem;
    }
    return NULL;    // 改为return NULL 因为可能未初始化。
}

int sem_post(sem_t *sem) {
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    OSSemPost(sem->uos_sem);
    return 0;
}

/*请求信号量，如果信号量为0则保持等待*/
/*疑问：怎么检测死锁？（may be）*/
/*参数2为该函数在获取不到信号时的阻塞超时时间，单位为systick，暂定为1000 */
int sem_wait(sem_t *sem) {
    INT8U *perr = NULL;
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    OSSemPend((sem->uos_sem), 1000, perr);
    return 0;
}

/*请求信号量，信号量有剩余才锁定信号量*/
int sem_trywait(sem_t *sem) {
    INT8U isLocked;
    INT8U *perr = NULL;
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    isLocked = OSSemAccept(sem->uos_sem);
    if (isLocked > 0) {
        return 0;
    } else {
        errno = EAGAIN;
        return -1;
    }
}


int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    INT8U *perr = NULL;
    int flag = 0;
    INT32U timeout_ms = 0;    // ljh: 把 int64u_t 改为INT32U
    struct timespec cur_time = {0};
    struct timespec rel_time = {0};
    if ((sem == NULL) || (abs_timeout == NULL)) {
        errno = EINVAL;
        return -1;
    }
    if ((abs_timeout->tv_sec < 0) || (abs_timeout->tv_nsec < 0)
        || (abs_timeout->tv_nsec >= 1000000000UL)) {
        errno = EINVAL;
        return -1;
    }

    flag = clock_gettime(CLOCK_REALTIME, &cur_time);
    if (flag != 0) {
        return -1;
    }

    rel_time.tv_sec = abs_timeout->tv_sec - cur_time.tv_sec;
    rel_time.tv_nsec = abs_timeout->tv_nsec - cur_time.tv_nsec;
    /*精确度暂时搁置 */
    timeout_ms = ((rel_time.tv_sec > 0) ? rel_time.tv_sec : 0) * 1000 +
                 ((rel_time.tv_nsec > 0) ? rel_time.tv_nsec : 0) / 1000000;

    OSSemPend(sem->uos_sem, timeout_ms, perr);
    if (perr == OS_ERR_TIMEOUT) {
        errno = ETIMEDOUT;
        return -1;
    }


    return 0;
}

/*删除由字符串名称命名的信号量*/
int sem_unlink(const char *name) {
    if (name == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (strlen(name) <= 0) {
        errno = ENOENT;
        return -1;
    }

    if (strlen(name) > SEM_NAME_MAX) {
        errno = ENAMETOOLONG;
        return -1;
    }
    for (int i = 0; i < OS_EVENT_TBL_SIZE; i++) {
        if (*name == OSEventTbl->OSEventName);
        /*此处存疑*/
    }

    errno = ENOSYS;
    return -1;
}


int sem_getvalue(sem_t *sem, int *sval) {
    INT8U perr;
    OS_SEM_DATA *data = OSMemGet(SemDataBuffer, &perr);  // ljh: 动态分配内存
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    perr = OSSemQuery(sem->uos_sem, data);
    sval = data->OSCnt;
    return 0;
}


int sem_close(sem_t *sem) {
    INT8U *perr = NULL;
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    OSSemDel(sem->uos_sem, OS_DEL_ALWAYS, perr);
    return 0;
}


int sem_destroy(sem_t *sem) {
    INT8U *perr = NULL;
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    OSSemDel(sem->uos_sem, OS_DEL_ALWAYS, perr);
    /*此处存疑*/
    if (perr != OS_ERR_TASK_WAITING) {
        return 0;
    } else {
        errno = EBUSY;
        return -1;
    }

}

