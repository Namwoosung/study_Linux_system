#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "synclib.h"
#include "shm.h"

#define NLOOPS 10
#define SIZE sizeof(long)

int update(long *ptr){
	return ((*ptr)++);
}

int main(){
	int i, counter, shmid, pid;
	char *ptr;
	
	//여기서는 shm.h를 include해서 shared memory를 생성
	//현재 예제에서는 다른 process와 공유하는 것이 아니라 child와 공유하는 것이므로 IPC_PRIVATE로 shared memory를 생성해도 됨
	//IPC_PRIVATE를 사용해서 shared memory를 만들었으면 IPC_RMID 작업 불필요
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
		
	//동기화 작업을 pipe를 통해 진행
	TELL_WAIT();
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if (pid > 0){
		//parent process
		for(i = 0; i < NLOOPS; i += 2){
			//생성해서 attach한 shared memory의 값을 반환 받고 증가
			if((counter = update((long *)ptr)) != i){
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			//할 일을 마쳤으니 child를 깨우고 자신은 wait
			TELL_CHILD();
			WAIT_CHILD();
		}
	}else{
		for(i = 1; i < NLOOPS; i += 2){
			//child process
			//parent가 일을 마칠 때 까지 대기
			WAIT_PARENT();
			//shared memory의 data를 반환받고 증가
			if((counter = update((long *)ptr)) != i){
				fprintf(stderr, "Counter mismach\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			//일을 마쳤으니 parent를 awake
			TELL_PARENT();
		}
	}

	//child와 parent모두 shared memory detach
	if(shmdt(ptr) < 0){
		perror("shmdt");
		exit(1);
	}
	//shared memory 반납은 parent 혹은 child 중 한 곳에서만 진행하면 되니, parent에서 진행하도록 설정
	if(pid > 0){
		if(shmctl(shmid, IPC_RMID, 0) < 0){
			perror("shmctl");
			exit(1);
		}
	}

	return 0;
}
