#include "face_time.h"

#ifndef FACE_MQUEUE_H
#define FACE_MQUEUE_H

#define DEFAULT_MQUEUE_SIZE  10240
#define DEFAULT_MAX_MSG_SIZE 1024

typedef void *mqd_t;
typedef INT32U ssize_t; // 按文档要求并非定义在此

struct mq_attr {
	long mq_flags;    /* message queue flags */
	long mq_maxmsg;   /* maximum number of messages */
	long mq_msgsize;  /* maximum message size */
	long mq_curmsgs;  /* number of messages currently queued */
};

mqd_t   mq_open(const char *name, int oflag, ...);
int     mq_close(mqd_t mqdes);
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
int     mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
int     mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat, struct mq_attr *omqstat);
int     mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);
ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio, const struct timespec *abs_timeout);
int     mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio, const struct timespec *abs_timeout);
int     mq_unlink(const char *name);

#endif