//기존 prodcons 예제를 semaphore.h가 아닌 직접 구현한 semlib2.h로 동작
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"
#include "prodcons.h"

//multi thread환경이므로 전역변수가 shared data역할
BoundedBufferType Buf;
sem_t EmptySem, FullSem, MutexSem;

//thread를 micro second 단위로 sleep 시키기 위한 함수
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


void Producer(void *dummy){
	int i, data;
	
	printf("Producer: Start....\n");
	
	//NLOOPS만큼 반복하며 Buf에 wirte, 동기화 진행
	for(i = 0; i < NLOOPS; i++){
		if(sem_wait(&EmptySem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if(sem_wait(&MutexSem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
	
		printf("Producer: Producing an item....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;
		
		if(sem_post(&MutexSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
		if(sem_post(&FullSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}

	printf("Producer: Produced %d items....\n", i);
	printf("Producer: %d items in buffer....\n", Buf.counter);

	pthread_exit(NULL);
}

void Consumer(void *dummy){
	int i, data;

	printf("Consumer: Start....\n");

	for(i = 0; i < NLOOPS; i++){
		if(sem_wait(&FullSem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if(sem_wait(&MutexSem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
	
		printf("Consumer: Consuming an item....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		if(sem_post(&MutexSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
		if(sem_post(&EmptySem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand() % 100) * 10000);
	}

	printf("Consumer: Consumed %d items....\n", i);
	printf("Consumer: %d items in buffer....\n", Buf.counter);

	pthread_exit(NULL);
}

int main(){
	pthread_t tid1, tid2;

	srand(0x8888);
	//전역변수로 선언한 semaphore들을 init
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

	printf("Main	: %d items in buffer....\n", Buf.counter);

	//사용한 semaphore들을 삭제
	if(sem_destroy(&EmptySem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&FullSem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&MutexSem) < 0){
		perror("sem_destroy");
	}

	return 0;
}
