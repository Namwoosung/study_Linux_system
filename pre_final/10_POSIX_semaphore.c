//	prodcons.h <- shared memory, semaphore, shared data�� ������ ���� ����
// ��� POSIX semaphore�� thread�� ����ȭ �̱⿡ ���� �����Ϳ� semaphore ��� shared memory�� �ƴ϶� ���� ������ ���� Ȱ��
// -> �� ���Ͽ���  SHM_KEY, SEM_KEY �� �Ⱦ���, �׳� buffer ����ü�� Ȱ����
#define MY_ID 15
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)

#define EMPTY_SEM_KEY (0x5000 + MY_ID)
#define FULL_SEM_KEY (0x6000 + MY_ID)
#define MUTEX_SEM_KEY (0x7000 + MY_ID)

#define NLOOPS 20

#define MAX_BUF 2

//buffer �� ĭ�� �����ϴ� data�� ����ü
typedef struct{
	int data;
} ItemType;

//buffer struct <- ���� ������ �����, �а� �� �ε����� ���� ������ ��� ����
typedef struct{
	ItemType buf[MAX_BUF];
	int in;
	int out;
	int counter;
} BoundedBufferType;

#define SHM_SIZE sizeof(BoundedBufferType); //shared memory�� ����



//	���� Ȱ��
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

//���� �����Ϳ� semaphore�� ���������� �����ؼ� ���
BoundedBufferType buf;
sem_t EmptySem, FullSem, MutexSem;


//thread�� micro second ������ sleep ��Ű�� ���� �Լ�
void ThreadUsleep(int usecs){
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	struct timespec ts;
	struct timeval tv;

	if(pthread_cond_init(&cond, NULL) < 0){
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if(pthread_mutex_init(&mutex, NULL) < 0){
		perror("pthread_mutex_init");
		exit(1);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;

	if(ts.tv_nsec >= 1000000000){
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if(pthread_mutex_lock(&mutex) < 0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}	
	if(pthread_cond_timedwait(&cond, &mutex, &ts) < 0){
		perror("pthread_cond_timewait");
		pthread_exit(NULL);
	}

	if(pthread_cond_destroy(&cond) < 0){
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if(pthread_mutex_destroy(&mutex) < 0){
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}


int main(){
	pthread_t tid1, tid2;

	srand(0x8888);

	//sem init
	if(sem_init(&EmptySem, 0, MAX_BUF) < 0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&FullSem, 0, 0) < 0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&MutexSem, 0, 1) < 0){
		perror("sem_init");
		exit(1);
	}

	//pthread_creat �ؼ� �� �� ��Ű��, pthread_join���� ��ٸ���
	if(pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}

	if(pthread_join(tid1, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}


	//����� semaphre ����
	if(sem_destroy(&EmptySem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&FullSem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&MutexSem) < 0){
		perror("sem_destroy");
	}
}


void Producer(void *dummy){ //thread�� �����ϴ� �Լ�, ������ main ���� �־�� ��, �̰� ���� �ڵ�ϱ� �׳� ���� ���ϰ� �Ʒ��� ���� ��
	int i, data;
	
	for(i = 0; i < NLOOPS; i++){
		//sem wait
		if(sem_wait(&EmptySem) < 0){
			perrror("sem_wait");
			pthread_exit(NULL);
		}
		if(sem_wait(&MutexSem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}

		// �� �� �ϰ� <- ���������� data�� random�� ���� �ְ�, buf�� �־���
		//assignment_8/prodcons.c


		//sem_post
		if(sem_post(&MutexSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data); //random�ϰ� ���鼭 ����

	}

	pthread_exit(NULL);
}



//�� �Դ� ö���� ���� �ڵ� ���� �ؾ��ϸ� assignment_08/dining2.c Ȯ��
