#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"

int main(){
	int shmid;
	char *ptr, *pData;
	int *pInt;
	
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
	//flag제어를 위한 pInt 할당
	pInt = (int *)ptr;
	//실제 data는 flag이후 작성할 것이므로 pointer 설정
	pData = ptr + sizeof(int);
	//data 작성
	sprintf(pData, "This is a request from %d.", getpid());
	//작성했으므로 flag를 1로 변경
	*pInt = 1;
	printf("Sent a request....");

	//다른 process에서 reply를 보낼 때 까지 대기
	while((*pInt) == 1)
			;
	
	printf("Received reply: %s\n", pData);
	
	//detach
	if(shmdt(ptr) < 0){
		perror("shmdt");
		exit(1);
	}

	return 0;
}
