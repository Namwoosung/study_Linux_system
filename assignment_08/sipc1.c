#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "semlib.h"
#include "sipc.h"

int main(){
	int shmid;
	char *ptr, *pData;
	int mutexSemid, fullSemid, emptySemid;
	
	//shared memory를 생성
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	//생성한 shared memory를 자신의 virtual address에 attach
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}

	if((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0){
		fprintf(stderr, "semInit failure\n");
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
	if(semWait(fullSemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}	
	//현재 shared memory인 critical section에 들어갈 수 있는 지 검사
	if(semWait(mutexSemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}	
	//flag를 없앴으니 data를 처음 위치부터 access
	pData = ptr;
	printf("Received request: %s.....", pData);
	//이후 자신의 reply로 넘길 내용을 pData에 덮어쓰고
	sprintf(pData, "This is a reply form %d.", getpid());
	printf("Replied.\n");
	//shared memory를 모두 사용했으니 signal
	if(semPost(mutexSemid) < 0){
		fprintf(stderr, "semPost failuer\n");
		exit(1);
	}
	if(semPost(emptySemid) < 0){
		fprintf(stderr, "semPost failuer\n");
		exit(1);
	}

	//아직 다른 process가 shared data를 읽는 중일 수 있으므로 1초 쉬어줌
	sleep(1);

	//이후 semaphore반납,  detach 및 shared memory 삭제 진행
	if(semDestroy(mutexSemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(emptySemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(fullSemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(shmdt(ptr) < 0){
		perror("shmdt");
		exit(1);
	}
	if(shmctl(shmid, IPC_RMID, 0) < 0){
		perror("shmctl");
		exit(1);
	}

	return 0;
}
