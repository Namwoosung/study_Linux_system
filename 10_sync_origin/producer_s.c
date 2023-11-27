//기존 producer.c를 수정해 동기화 작업을 수행
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
	//공유할 shared memory를 생성
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	if((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	//synchronization을 위한 semaphore 3개를 각각 생성
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
	//동일하게 NLOOP만큼 반복하며 write를 진행
	//여기서 동기화를 위해 wait과 post를 사용
	srand(0x8888);
	for(i = 0; i < NLOOPS; i++){
		//buffer에 빈 공간이 있으면 진입, 없으면 wait
		if(semWait(emptySemid) < 0){
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		//critical section에 진입할 수 있으면 진입, 없으면 wait
		if(semWait(mutexSemid) < 0){
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}		
		
		printf("producer: Producing an item....\n");
		data = (rand() % 100) * 10000;
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;
		//사용하고 critical section의 semaphore 반납
		if(semPost(mutexSemid) < 0){
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		//data를 하나 write했으니 full semaphore + 1
		if(semPost(fullSemid) < 0){
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		
		usleep(data);
	}
	printf("Producer: Produced %d items.....\n", i);

	sleep(2);
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	//producer_s 가 늦게 끝난다고 가정 -> 생성한 semaphore와 shared data를 삭제
	if(semDestroy(emptySemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(fullSemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(mutexSemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(shmctl(shmid, IPC_RMID, 0) < 0){
		perror("shmctl");
		exit(1);
	}
	
	return 0;
}
