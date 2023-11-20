#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE (SHM_R | SHM_W)

//data segment에 저장되는 전역변수로 배열 선언
char Array[ARRAY_SIZE];

int main(){
	int shmid;
	char *ptr, *shmptr;
	
	//heap 영역에 저장되는 ptr 동적배열
	if((ptr = (char *)malloc(MALLOC_SIZE)) == NULL){
		perror("malloc");
		exit(1);
	}
	//shared memory를 생성
	//첫 파라미터인 key에 IPC_PRIVATE를 넘겨주면 해당 shared memory는 다른 process와 공유하지 않고, 자신만 사용 가능
	// -> 이 경우 IPC_RMID 작업을 안 해주어도 process 종료시 shared memory 반납
	//대신 key가 존재하지 않으니 다른 process와 공유하는 목적으로 사용 불가
	//fork()를 통해 자식 process를 만든다면 자식과는 공유 가능
	if((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	//생성한 shared memory를 자신의 virtual address상에 attach
	if((shmptr = shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	//data segment
	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]);
	//satck
	printf("Stack around %p\n", &shmid);
	//heap
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE);
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE);

	//printf 출력 결과 shared memory는 virtual address에서 stack과 heap 사이 위치에 attach 되는 것을 확인 가능
	

	//virtaul address에서 shared memory detach
	if(shmdt(shmptr) < 0){
		perror("shmdt");
		exit(1);
	}
	//shared memory 삭제
	if(shmctl(shmid, IPC_RMID, 0) < 0){
		perror("shmctl");
		exit(1);
	}

	return 0;
}
