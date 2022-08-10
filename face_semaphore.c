#include<face_semaphore.h>
#include <ucos_ii.h>
#include<face_errno.h>
#include<stdlib.h>

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
	       3. 初始化信号量已达信号量限制？
	 */
	sem->uos_sem = OSSemCreate(value);
	return 0;
}

sem_t *sem_open(const char *name, int oflag, ...){

}


