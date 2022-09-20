#include "ucos_ii.h"
#include "face_time.h"

#ifndef FACE_SCHED_H
#define FACE_SCHED_H

typedef INT32U pid_t;    /* should have been imported from head file <sys/types.h> */

/* The scheduling policy */
#define SCHED_OTHER 0    // 分时调度策略，优先级无效（即最低），保证其它调度策略的线程可以抢占CPU，not supported
#define SCHED_FIFO  1    // 优先级抢占，即μc/OS默认的调度方式，supported
#define SCHED_RR    2    // 时间片轮转调度，在优先级抢占的基础上限制最大运行时间，supported
#define SCHED_CFS   3    // 完全公平调度策略，not supported
#define SCHED_SPORADIC 4    // 零星的非周期行为，可以让该Thread在短期内以高优先级执行，其它时间内以低优先级执行，not supported

#define MIN_PRIO 1
#define MAX_PRIO 99

struct sched_param {
	int      sched_priority; /* process execution scheduling priority */
	INT32U   slice;          /* time slice in SCHED_RR mode (ms), change to 32 unsigned int in that no 64bits supported */
};

int sched_yield(void);
int sched_getscheduler(pid_t pid);
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
int sched_getparam(pid_t pid, struct sched_param *param);
int sched_setparam(pid_t pid, const struct sched_param *param);
int sched_get_priority_min(int policy);
int sched_get_priority_max(int policy);
int sched_rr_get_interval(pid_t pid, struct timespec *interval);

#endif