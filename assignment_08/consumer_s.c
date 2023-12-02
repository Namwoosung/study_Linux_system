//기존 consumer.c를 수정해 동기화 작업을 수행
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

int main(){
	BoundedBufferType *pBuf;
	int shmid, i, data;
	int emptySemid, fullSemid, mutexSemid;
	//shared data와 semaphore 생성
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	if((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	
	if((emptySemid = semInit(EMPTY_SEM_KEY)) < 0){
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if((fullSemid = semInit(FULL_SEM_KEY)) < 0){
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}	
	if((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0){
		fprintf(stderr, "semInit failure\n");
		exit(1);	
	}
	//consumer_s가 먼저 시행된다고 가정, semaphore 값을 초기화
	if(semInitValue(emptySemid, MAX_BUF) < 0){
		fprintf(stderr, "semInitValue failure\n");
		exit(1);	
	}
	if(semInitValue(fullSemid, 0) < 0){
		fprintf(stderr, "semInitValue failure\n");
		exit(1);	
	}
	if(semInitValue(mutexSemid, 1) < 0){
		fprintf(stderr, "semInitValue failure\n");
		exit(1);	
	}
	
	//동일하게 NLOOPS만큼 돌면서 read
	srand(0x9999);
	for(i = 0; i < NLOOPS; i++){
		//차있는 공간에 진입, 없으면 wait
		if(semWait(fullSemid) < 0){
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		//critical section에 들어가면서 semaphore -1, 못하면 wait
		if(semWait(mutexSemid) < 0){
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}		
		
		printf("Consumer: Consuming an item....\n");
		data = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;

		if(semPost(mutexSemid) < 0){
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		if(semPost(emptySemid) < 0){
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		
		usleep((rand() % 100) * 10000);
	}
	printf("Consumer: Consumer %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);

	return 0;
}
