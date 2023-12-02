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
	
	//shared memory의 id를 가져옴(없으면 생성)
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmid");
		exit(1);
	}
	//virtual address에 attach
	if((ptr = shmat(shmid, 0 ,0)) == (void *)-1){
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
	//sipc2가 먼저 시행된다고 가정, 여기서 semaphore value를 설정
	//여기서는 shared memory에 한 번 씩 data를 번갈아 쓰기 맞추서 value init
	if(semInitValue(mutexSemid, 1) < 0){
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if(semInitValue(emptySemid, 1) < 0){
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if(semInitValue(fullSemid, 0) < 0){
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	
	//빈공간을 하나 차지하고 시작
	if(semWait(emptySemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//data 작성 이전 wait하고 진입
	if(semWait(mutexSemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//기존에 flag부분을 없앴으므로 data는 shared memory처음 부분부터 작성 가능
	pData = ptr;
	//data 작성
	sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request....");
	//data 작성을 마쳤으니 signal
	if(semPost(mutexSemid) < 0){
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	if(semPost(fullSemid) < 0){
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	

	if(semWait(emptySemid) < 0){
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	//다른 process가 작성한 data를 읽어오기 전에 wait하고 진입	
	if(semWait(mutexSemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	printf("Received reply: %s\n", pData);
	//읽었으니 signal
	if(semPost(mutexSemid) < 0){
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	if(semPost(fullSemid) < 0){
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	
	//detach
	if(shmdt(ptr) < 0){
		perror("shmdt");
		exit(1);
	}

	return 0;
}
