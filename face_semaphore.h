#define SEM_NSEMS_MAX   32767      /*信号量数量限制，数量暂定*/
#define SEM_VALUE_MAX   INT_MAX  /*信号量值的限制*/
#define SEM_FAILED ((sem_t *)0)

/*sem_t应该由文件描述符实现？*/

typedef struct {
	void *uos_sem;
} sem_t;

int    sem_init(sem_t *sem, int pshared, unsigned int value);
sem_t *sem_open(const char *name, int oflag, ...);
int    sem_post(sem_t *sem);
int    sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
int    sem_trywait(sem_t *sem);
int    sem_unlink(const char *name);
int    sem_wait(sem_t *sem);
int    sem_getvalue(sem_t *sem, int *sval);
int    sem_close(sem_t *sem);
int    sem_destroy(sem_t *sem);