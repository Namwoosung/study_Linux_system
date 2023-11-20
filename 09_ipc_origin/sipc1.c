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
	
	//shared memory를 생성하고 id를 받음
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	//생성한 shared memory를 자신의 virtual address에 attach
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	//shaded memory를 가르키는 pointer와 동일한 지점을 가르키는 int형 pointer를 할당
	//ptr은 char *이므로 1byte씩 읽지만, pInt는 int *이므로 4byte씩 읽을 것
	pInt = (int *)ptr;

	//동기화 제어를 위한 임시 장치, shared memory의 처음 4byte는 flag로 사용
	//sipc2가 write하면 1로 만들고, sipc1이  write하면 0으로 만듦
	//flag가 0이면 아직 child가 write하지 않은 것이므로 무한 대기
	while ((*ptr) == 0)
			;
	//위 처럼 구현하면 사실 굉장히 비효율적
	//이런식으로 구현하게 되면 이 process는 waiting상태로 이동하지 않음 -> run하다가 자신의 timer를 모두 소진하면
	//timer interrupt가 발생해 ready 상태로 돌아감 -> 이후 다시 스케줄링 되어 run상태가 되면 다시 무한 반복하다가 ready로 돌아가기를 반복
	//-> 이 process가 CPU를 점거하는 동안은 CPU가 낭비되는 busy-waiting상태인 것
	//따라서 정상동작하게 하기 위해서는 semaphore와 같은 lock을 제대로 구현해서 다른 process의 write를 기다려야 되는 상황이 오면
	//waiting상태로 이동하게 하고, 이후 wrtie가 발생하는 시점에 waiting에서 ready로 갈 수 있도록 구현하는 것이 옳음
	
	//*ptr이 1이 되어 반복을 탈출하면 data가 쓰였다는 의미
	//-> data를 가져오고 print로 출력
	pData = ptr + sizeof(int);
	printf("Received request: %s.....", pData);
	//이후 자신의 reply로 넘길 내용을 pData에 덮어쓰고
	sprintf(pData, "This is a reply form %d.", getpid());
	//flag를 바꿔줌(다른 process는 flag가 0이 될때까지 무한반복하며 기다리고 있을 것)
	*pInt = 0;
	printf("Replied.\n");

	//아직 다른 process가 shared data를 읽는 중일 수 있으므로 1초 쉬어줌
	sleep(1);

	//이후 detach 및 shared memory 삭제 진행
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
