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
	
	//shared memory�� �����ϰ� id�� ����
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	//������ shared memory�� �ڽ��� virtual address�� attach
	if((ptr = shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	//shaded memory�� ����Ű�� pointer�� ������ ������ ����Ű�� int�� pointer�� �Ҵ�
	//ptr�� char *�̹Ƿ� 1byte�� ������, pInt�� int *�̹Ƿ� 4byte�� ���� ��
	pInt = (int *)ptr;

	//����ȭ ��� ���� �ӽ� ��ġ, shared memory�� ó�� 4byte�� flag�� ���
	//sipc2�� write�ϸ� 1�� �����, sipc1��  write�ϸ� 0���� ����
	//flag�� 0�̸� ���� child�� write���� ���� ���̹Ƿ� ���� ���
	while ((*ptr) == 0)
			;
	//�� ó�� �����ϸ� ��� ������ ��ȿ����
	//�̷������� �����ϰ� �Ǹ� �� process�� waiting���·� �̵����� ���� -> run�ϴٰ� �ڽ��� timer�� ��� �����ϸ�
	//timer interrupt�� �߻��� ready ���·� ���ư� -> ���� �ٽ� �����ٸ� �Ǿ� run���°� �Ǹ� �ٽ� ���� �ݺ��ϴٰ� ready�� ���ư��⸦ �ݺ�
	//-> �� process�� CPU�� �����ϴ� ������ CPU�� ����Ǵ� busy-waiting������ ��
	//���� �������ϰ� �ϱ� ���ؼ��� semaphore�� ���� lock�� ����� �����ؼ� �ٸ� process�� write�� ��ٷ��� �Ǵ� ��Ȳ�� ����
	//waiting���·� �̵��ϰ� �ϰ�, ���� wrtie�� �߻��ϴ� ������ waiting���� ready�� �� �� �ֵ��� �����ϴ� ���� ����
	
	//*ptr�� 1�� �Ǿ� �ݺ��� Ż���ϸ� data�� �����ٴ� �ǹ�
	//-> data�� �������� print�� ���
	pData = ptr + sizeof(int);
	printf("Received request: %s.....", pData);
	//���� �ڽ��� reply�� �ѱ� ������ pData�� �����
	sprintf(pData, "This is a reply form %d.", getpid());
	//flag�� �ٲ���(�ٸ� process�� flag�� 0�� �ɶ����� ���ѹݺ��ϸ� ��ٸ��� ���� ��)
	*pInt = 0;
	printf("Replied.\n");

	//���� �ٸ� process�� shared data�� �д� ���� �� �����Ƿ� 1�� ������
	sleep(1);

	//���� detach �� shared memory ���� ����
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
