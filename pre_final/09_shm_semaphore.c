#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h" //for shared memory
#include "semlib.h" //for semaphorer <- System V semaphore를 POSIX 구문과 비슷하게 구현한 라이브러리

//	shm.h
#define MY_ID 15
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_SIZE 1024
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)

// shared memory + Syetem V semaphore
int main(){
	int shmid;
	char *ptr, *pData;
	int mutexSemid, fullSemid, emptySemid;

	//shared memory 설정
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){ //shared memory get
		perror("shmid");
		exit(1);
	}
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1){ //shared memory를 가져오기
		perror("shmat");
		exit(1);
	} //ptr에 data를 읽고, 쓰기 가능

	
	//semaphore 생성
	if((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0){
		fprintf(stderr, "semInit failure\n");
	}
	if((emptySemid = semInit(EMPTY_SEM_KEY)) < 0){
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if((fullSemid = semInit(FULL_SEM_KEY)) < 0){
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	//value init은 semaphore 용도에 맞게 <- initvalue는 먼저 시행되는 process에서만 시행해주면 됨
	if(semInitvalue(mutexSemid, 1) < 0){
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

	

	//이후 semaphore반납, shared memory 삭제 <- 이건 나중에 끝나는 process에서 한 번만 진행해주면 됨
	if(semDestroy(mutexSemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(emptySemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(semDestroy(fullSemid) < 0){
		fprintf(stderr, "semDestroy failure\n");
	}
	if(shmctl(shmid, IPC_RMID, 0) < 0){
		perror("shmctl");
		exit(1);
	}



	return 0;
}
