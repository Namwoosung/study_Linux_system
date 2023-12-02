//Posix semaphoer�� mutex�� condition varibale�� ����
#include <pthread.h>

typedef struct {
	//semaphore ��
	int sval;
	//shared data�� sval�� ��ȣ
	pthread_mutex_t mutex;
	//wait�� ��⸦ �����ϱ� ���� condition variable
	pthread_cond_t cond;
} sem_t;

int sem_init(sem_t *sem, int pshared, int value);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_getvalue(sem_t *sem, int *sval);
int sem_destroy(sem_t *sem);
