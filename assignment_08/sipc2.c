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
	
	//shared memory�� id�� ������(������ ����)
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmid");
		exit(1);
	}
	//virtual address�� attach
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
	//sipc2�� ���� ����ȴٰ� ����, ���⼭ semaphore value�� ����
	//���⼭�� shared memory�� �� �� �� data�� ������ ���� ���߼� value init
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
	
	//������� �ϳ� �����ϰ� ����
	if(semWait(emptySemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//data �ۼ� ���� wait�ϰ� ����
	if(semWait(mutexSemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//������ flag�κ��� �������Ƿ� data�� shared memoryó�� �κк��� �ۼ� ����
	pData = ptr;
	//data �ۼ�
	sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request....");
	//data �ۼ��� �������� signal
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
	//�ٸ� process�� �ۼ��� data�� �о���� ���� wait�ϰ� ����	
	if(semWait(mutexSemid) < 0){
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	printf("Received reply: %s\n", pData);
	//�о����� signal
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
