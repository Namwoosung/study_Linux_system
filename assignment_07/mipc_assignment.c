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
	
	//���⼭�� shm.h�� include�ؼ� shared memory�� ����
	//���� ���������� �ٸ� process�� �����ϴ� ���� �ƴ϶� child�� �����ϴ� ���̹Ƿ� IPC_PRIVATE�� shared memory�� �����ص� ��
	//IPC_PRIVATE�� ����ؼ� shared memory�� ��������� IPC_RMID �۾� ���ʿ�
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
		
	//����ȭ �۾��� pipe�� ���� ����
	TELL_WAIT();
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if (pid > 0){
		//parent process
		for(i = 0; i < NLOOPS; i += 2){
			//�����ؼ� attach�� shared memory�� ���� ��ȯ �ް� ����
			if((counter = update((long *)ptr)) != i){
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			//�� ���� �������� child�� ����� �ڽ��� wait
			TELL_CHILD();
			WAIT_CHILD();
		}
	}else{
		for(i = 1; i < NLOOPS; i += 2){
			//child process
			//parent�� ���� ��ĥ �� ���� ���
			WAIT_PARENT();
			//shared memory�� data�� ��ȯ�ް� ����
			if((counter = update((long *)ptr)) != i){
				fprintf(stderr, "Counter mismach\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			//���� �������� parent�� awake
			TELL_PARENT();
		}
	}

	//child�� parent��� shared memory detach
	if(shmdt(ptr) < 0){
		perror("shmdt");
		exit(1);
	}
	//shared memory �ݳ��� parent Ȥ�� child �� �� �������� �����ϸ� �Ǵ�, parent���� �����ϵ��� ����
	if(pid > 0){
		if(shmctl(shmid, IPC_RMID, 0) < 0){
			perror("shmctl");
			exit(1);
		}
	}

	return 0;
}
