#define N_MESSAGES 128
#define MAX_NQUEUES 64
#define MAX_NAME_LEN 64

#include "ucos_ii.h"
#include "face_mqueue.h"
#include "face_time.h"

#include "face_errno.h"

static int index = 0;
struct core_mqd_t {
	mqd_t mq_holder;
	char name[MAX_NAME_LEN];
	INT8U link_ctr;
} *MQ_TABLE[MAX_NQUEUES];

static int equal(const char *src, const char *des) {
	while (*src != '\0' && *des != '\0' && *src++ == *des++);
	if (*src == *des) return 1;
	else return 0;
}

int find(const char *name) {
	for (int i = 0; i < index; i++) {
		if (equal((*MQ_TABLE[i]).name, name)) return i;
	}
	return -1;
}

mqd_t   mq_open(const char *name, int oflag, ...) {
	int i;
	if ((i = find(name)) != -1) {
		(*MQ_TABLE[i]).link_ctr++;
		return (*MQ_TABLE[i]).mq_holder;
	}
	void* MsgGrp[N_MESSAGES];
	OS_EVENT *mq;
	mq = (mqd_t) OSQCreate(MsgGrp, N_MESSAGES);
	struct core_mqd_t new_mq;
	new_mq.link_ctr = 0;
	new_mq.mq_holder = mq;
	OS_MemCopy(new_mq.name, name, OS_StrLen(name) + 1);
	MQ_TABLE[index++] = &new_mq;
	return mq;
}

int     mq_close(mqd_t mqdes) {
	INT8U err = 0;
	OSQDel((OS_EVENT*) mqdes, 0, &err);
	return err;
}

ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio) {
	// need to check msg_len argument
	INT8U err = 0;
	char* s = (char*)OSQPend((OS_EVENT*)mqdes, 0, &err);
	msg_len = OS_StrLen(s);
	OS_MemCopy(msg_ptr, s, msg_len + 1);
	if (err != OS_ERR_NONE) return 0;
	return msg_len;
}

int     mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio) {
	INT8U err = OSQPostFront((OS_EVENT*)mqdes, msg_ptr);
	if (err != OS_ERR_NONE) return 0;
	return msg_len;
}

int     mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat) {
	return 0;
}

int     mq_getattr(mqd_t mqdes, struct mq_attr *mqstat) {
	return 0;
}

ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio, const struct timespec *abs_timeout) {
	INT8U err = 0;
	char* s = (char*)OSQPend((OS_EVENT*)mqdes, timespce2ticks(abs_timeout), &err);
	msg_len = OS_StrLen(s);
	OS_MemCopy(msg_ptr, s, msg_len + 1);
	if (err != OS_ERR_NONE) return 0;
	return msg_len;
}

int     mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio, const struct timespec *abs_timeout) {
	INT8U err = OSQPost((OS_EVENT*)mqdes, msg_ptr);
	if (err != OS_ERR_NONE) return 0;
	return msg_len;
}

int     mq_unlink(const char *name) {
	int i;
	if ((i = find(name)) != -1) {
		if (--(*MQ_TABLE[i]).link_ctr == 0) mq_close((*MQ_TABLE[i]).mq_holder);
	}
	else return EINVAL;
	return 0;
}