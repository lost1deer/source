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
	/* Sharing between processes is not supported. (ע���ò�����FACE�ĵ��в�δ�ᵽ)*/
	if (pshared != 0) {
		errno = ENOTSUP;
		return -1;
	}
	/*δ��ɣ�
	 *     1. �ý���ȱ�ٳ�ʼ���ź������ʵ�Ȩ�ޡ���2022.08.08��
	 *     2. ����pshared�����������Ĳ������漰����������spin locks��
	       3. ��ʼ���ź����Ѵ��ź������ƣ�
	 */
	sem->uos_sem = OSSemCreate(value);
	return 0;
}

sem_t *sem_open(const char *name, int oflag, ...){

}


