#include<face_semaphore.h>
#include <ucos_ii.h>
#include<face_errno.h>
#include<stdlib.h>
#include<face_time.h>

#define SEM_NAME_MAX 1024

int sem_init(sem_t *sem, int pshared, unsigned int value){
	
	if ((sem == NULL) || value > SEM_VALUE_MAX){
		errno = EINVAL;
		return -1;
	}
	if ((value == 0) || value > SEM_NSEMS_MAX){
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

sem_t *sem_open(const char *name, int oflag, ...){

}

int sem_post(sem_t *sem){
	if (sem == NULL) {
		errno = EINVAL;
		return -1;
	}
	OSSemPost(sem->uos_sem);
	return 0;
}

/*请求信号量，如果信号量为0则保持等待*/
/*疑问：怎么检测死锁？（may be）*/
/*参数2为该函数在获取不到信号时的阻塞超时时间，单位为systick，暂定为0 */
int sem_wait(sem_t *sem){
	INT8U *perr = NULL;
	if (sem == NULL) {
		errno = EINVAL;
		return -1;
	}
	OSSemPend((sem->uos_sem), 0, perr);
	return 0;
}

/*请求信号量，信号量有剩余才锁定信号量*/
int sem_trywait(sem_t *sem){
	INT8U isLocked;
	INT8U *perr = NULL;
	if (sem == NULL) {
		errno = EINVAL;
		return -1;
	}
	isLocked=OSSemAccept(sem->uos_sem);
	if (isLocked>0){
       return 0;
	}
	else{
		errno = EAGAIN;
		return -1;
	}
}


int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout){
	INT8U *perr = NULL;
	int flag = 0;
	uint64_t timeout_ms = 0;
	struct timespec cur_time = { 0 };
	struct timespec rel_time = { 0 };
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
int sem_unlink(const char *name){
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

	errno = ENOSYS;
	return -1;
}


int sem_getvalue(sem_t *sem, int *sval){
	if ((sem == NULL) || (sval == NULL)) {
		errno = EINVAL;
		return -1;
	}


}


int sem_close(sem_t *sem){

}


int sem_destroy(sem_t *sem){
	INT8U *perr=NULL;
	if (sem == NULL) {
		errno = EINVAL;
		return -1;
	}
	OSSemDel(sem->uos_sem, OS_DEL_ALWAYS, perr);
	/*此处存疑*/
	if (perr != OS_ERR_TASK_WAITING){
		return 0;
	}
	else{
		errno = EBUSY;
		return -1;
	}

}

