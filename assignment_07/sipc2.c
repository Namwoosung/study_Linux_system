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
	//flag��� ���� pInt �Ҵ�
	pInt = (int *)ptr;
	//���� data�� flag���� �ۼ��� ���̹Ƿ� pointer ����
	pData = ptr + sizeof(int);
	//data �ۼ�
	sprintf(pData, "This is a request from %d.", getpid());
	//�ۼ������Ƿ� flag�� 1�� ����
	*pInt = 1;
	printf("Sent a request....");

	//�ٸ� process���� reply�� ���� �� ���� ���
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
