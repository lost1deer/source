#define N_MESSAGES 128

#include<ucos_ii.h>
#include<face_mqueue.h>
#include<face_time.h>

mqd_t   mq_open(const char *name, int oflag, ...) {
	void* MsgGrp[N_MESSAGES];
	OS_EVENT *mq;
	mq = (mqd_t) OSQCreate(MsgGrp, N_MESSAGES);
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
	char* s = (char*)OSQPend((OS_EVENT*)mqdes, 0, &err);
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

}