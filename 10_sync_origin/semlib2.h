//Posix semaphoer를 mutex와 condition varibale로 구현
#include <pthread.h>

typedef struct {
	//semaphore 수
	int sval;
	//shared data인 sval을 보호
	pthread_mutex_t mutex;
	//wait시 대기를 구현하기 위한 condition variable
	pthread_cond_t cond;
} sem_t;

int sem_init(sem_t *sem, int pshared, int value);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_getvalue(sem_t *sem, int *sval);
int sem_destroy(sem_t *sem);
