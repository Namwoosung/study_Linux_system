//	prodcons.h <- shared memory, semaphore, shared data를 정의해 놓은 파일
// 사실 POSIX semaphore는 thread간 동기화 이기에 공유 데이터와 semaphore 모두 shared memory가 아니라 전역 변수를 통해 활용
// -> 이 파일에서  SHM_KEY, SEM_KEY 다 안쓰고, 그냥 buffer 구조체만 활용함
#define MY_ID 15
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)

#define EMPTY_SEM_KEY (0x5000 + MY_ID)
#define FULL_SEM_KEY (0x6000 + MY_ID)
#define MUTEX_SEM_KEY (0x7000 + MY_ID)

#define NLOOPS 20

#define MAX_BUF 2

//buffer 한 칸을 차지하는 data의 구조체
typedef struct{
	int data;
} ItemType;

//buffer struct <- 버퍼 공간을 만들고, 읽고 쓸 인덱스와 현재 개수를 담고 있음
typedef struct{
	ItemType buf[MAX_BUF];
	int in;
	int out;
	int counter;
} BoundedBufferType;

#define SHM_SIZE sizeof(BoundedBufferType); //shared memory는 버퍼



//	실제 활용
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

//공유 데이터와 semaphore를 전역변수로 선언해서 사용
BoundedBufferType buf;
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

	//pthread_creat 해서 할 일 시키고, pthread_join으로 기다리기
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


	//사용한 semaphre 삭제
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


void Producer(void *dummy){ //thread가 수행하는 함수, 원래는 main 위에 있어야 됨, 이건 공부 코드니까 그냥 보기 편하게 아래로 내린 것
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

		// 할 거 하고 <- 예제에서는 data에 random한 값을 넣고, buf에 넣었음
		//assignment_8/prodcons.c


		//sem_post
		if(sem_post(&MutexSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data); //random하게 쉬면서 동작

	}

	pthread_exit(NULL);
}



//밥 먹는 철학자 문제 코드 참고 해야하면 assignment_08/dining2.c 확인
