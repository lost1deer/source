#ifndef   FACE_SIGNAL_H
#define   FACE_SIGNAL_H
#include<signal.h>
#include<ucos_ii.h>
#include<face_pthread.h>
/*Ϊsigev_notify��ֵ�������·��ų��������ڳ�����ֵĿǰ�����Ӧ���趨Ϊ���٣�2022.08.03)*/
#define SIGEV_NONE    1u          
#define SIGEV_SIGNAL  2u         /*Ŀ���¼�����ʱ���ɾ��г�����ֵ���Ŷ��ź�*/
#define SIGEV_THREAD  3u         /*����֪ͨ����ִ��֪ͨ*/

union sigval{
	int sival_int;            /*�������ź���*/
	void *sival_ptr;          /*ָ�����ź���*/
};

struct sigevent
{
	int sigev_notify;
	int sigev_signo;
	union sigval sigev_value;
	void (*sigev_notify_function)(union sigval);
	pthread_attr_t *sigev_notify_attributes;
};

#endif