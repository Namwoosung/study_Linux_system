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
	
	//shared memory�� ����
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	//������ shared memory�� �ڽ��� virtual address�� attach
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
	//���� shared memory�� critical section�� �� �� �ִ� �� �˻�
	if(semWait(mutexSemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}	
	//flag�� �������� data�� ó�� ��ġ���� access
	pData = ptr;
	printf("Received request: %s.....", pData);
	//���� �ڽ��� reply�� �ѱ� ������ pData�� �����
	sprintf(pData, "This is a reply form %d.", getpid());
	printf("Replied.\n");
	//shared memory�� ��� ��������� signal
	if(semPost(mutexSemid) < 0){
		fprintf(stderr, "semPost failuer\n");
		exit(1);
	}
	if(semPost(emptySemid) < 0){
		fprintf(stderr, "semPost failuer\n");
		exit(1);
	}

	//���� �ٸ� process�� shared data�� �д� ���� �� �����Ƿ� 1�� ������
	sleep(1);

	//���� semaphore�ݳ�,  detach �� shared memory ���� ����
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
