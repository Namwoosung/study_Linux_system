#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h" //for shared memory
#include "semlib.h" //for semaphorer <- System V semaphore�� POSIX ������ ����ϰ� ������ ���̺귯��

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

	//shared memory ����
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){ //shared memory get
		perror("shmid");
		exit(1);
	}
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1){ //shared memory�� ��������
		perror("shmat");
		exit(1);
	} //ptr�� data�� �а�, ���� ����

	
	//semaphore ����
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
	//value init�� semaphore �뵵�� �°� <- initvalue�� ���� ����Ǵ� process������ �������ָ� ��
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

	

	//���� semaphore�ݳ�, shared memory ���� <- �̰� ���߿� ������ process���� �� ���� �������ָ� ��
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
