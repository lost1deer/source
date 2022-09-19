#include "face_sched.h"
#include "face_pthread.h"

#include "face_errno.h"

/*
1. uc任务优先级由0到OS_LOWEST_PRIO最多共 OS_LOWEST_PRIO + 1 个任务
2. 优先级越小则越先执行
*/

int errno;    /* should have been imported from errno.h */

static inline pthread_t _pid_to_pthread(pid_t pid)
{
	if (pid == -1) {
		return NULL;
	}

	/* todo, not surpport  */
	return (pthread_t)(uintptr_t)pid;
}

/* Convert pid to ptcb of the thread. */
static inline pthread_tcb_t* sched_get_ptcb(pid_t pid)
{
	pthread_t thread = 0;

	thread = _pid_to_pthread(pid);
	if (thread == 0) {
		thread = pthread_self();
	}

	return __pthread_get_tcb(thread);
}

static inline get_priority_min(INT32U policy) {
	return MIN_PRIO;
}

static inline get_priority_max(INT32U policy) {
	return MAX_PRIO;
}

int sched_get_priority_min(int policy) {
	if (policy != SCHED_FIFO || policy != SCHED_RR) {
		errno = EINVAL;
		return -1;
	}
	return get_priority_min(policy);
}

int sched_get_priority_max(int policy) {
	if (policy != SCHED_FIFO || policy != SCHED_RR) {
		errno = EINVAL;
		return -1;
	}
	return get_priority_max(policy);
}

int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)
{
	int formerpolicy = 0;
	pthread_tcb_t *ptcb = NULL;

	ptcb = sched_get_ptcb(pid);
	if (ptcb == NULL) {
		errno = ESRCH;
		return -1;
	}

	if (policy != SCHED_FIFO || policy != SCHED_RR) {
		errno = EINVAL;
		return -1;
	}

	if ((param == NULL) || (param->sched_priority < get_priority_min(policy))
		|| (param->sched_priority > get_priority_max(policy))) {
		errno = EINVAL;
		return -1;
	}
	formerpolicy = ptcb->attr.schedpolicy;
	// 不知道这里要不要关中断
	ptcb->attr.schedpolicy = policy;
	ptcb->attr.schedparam.sched_priority = param->sched_priority;
	if (policy == SCHED_RR) {
		ptcb->attr.schedparam.slice = param->slice;
	}

	return formerpolicy;
}

int sched_getscheduler(pid_t pid)
{
	pthread_tcb_t *ptcb = NULL;

	ptcb = sched_get_ptcb(pid);
	if (ptcb == NULL) {
		errno = ESRCH;
		return -1;
	}

	return ptcb->attr.schedpolicy;
}

int sched_setparam(pid_t pid, const struct sched_param *param)
{
	pthread_tcb_t *ptcb = NULL;

	ptcb = sched_get_ptcb(pid);
	if (ptcb == NULL) {
		errno = ESRCH;
		return -1;
	}

	if ((param == NULL)) {
		errno = EINVAL;
		return -1;
	}

	ptcb->attr.schedparam.sched_priority = param->sched_priority;
	ptcb->attr.schedparam.slice = param->slice;

	return 0;
}

int sched_getparam(pid_t pid, struct sched_param *param)
{
	pthread_tcb_t *ptcb = NULL;

	if (param == NULL) {
		errno = EINVAL;
		return -1;
	}

	ptcb = sched_get_ptcb(pid);
	if (ptcb == NULL) {
		errno = ESRCH;
		return -1;
	}

	param->sched_priority = ptcb->attr.schedparam.sched_priority;// OSTCBCur->OSTCBPrio;
	param->slice = ptcb->attr.schedparam.slice;

	return 0;
}

int sched_yield(void)
{
	OS_Sched();
	return 0;
}

int sched_rr_get_interval(pid_t pid, struct timespec *interval)
{
	pthread_tcb_t *ptcb = NULL;
	uint8_t policy = 0;
	uint32_t slice_ms = 0;

	if (interval == NULL) {
		errno = EINVAL;
		return -1;
	}

	// 如果pid为0则获取当前线程的时间片
	ptcb = sched_get_ptcb(pid);
	if (ptcb == NULL) {
		errno = ESRCH;
		return -1;
	}

	policy = ptcb->attr.schedpolicy;
	if (policy != SCHED_RR) {
		errno = EINVAL;
		return -1;
	}

	slice_ms = ptcb->attr.schedparam.slice;

	interval->tv_sec = slice_ms / 1000;
	interval->tv_nsec = (slice_ms % 1000) * 1000000;

	return 0;
}
