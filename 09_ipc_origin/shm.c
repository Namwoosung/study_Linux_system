#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE (SHM_R | SHM_W)

//data segment�� ����Ǵ� ���������� �迭 ����
char Array[ARRAY_SIZE];

int main(){
	int shmid;
	char *ptr, *shmptr;
	
	//heap ������ ����Ǵ� ptr �����迭
	if((ptr = (char *)malloc(MALLOC_SIZE)) == NULL){
		perror("malloc");
		exit(1);
	}
	//shared memory�� ����
	//ù �Ķ������ key�� IPC_PRIVATE�� �Ѱ��ָ� �ش� shared memory�� �ٸ� process�� �������� �ʰ�, �ڽŸ� ��� ����
	// -> �� ��� IPC_RMID �۾��� �� ���־ process ����� shared memory �ݳ�
	//��� key�� �������� ������ �ٸ� process�� �����ϴ� �������� ��� �Ұ�
	//fork()�� ���� �ڽ� process�� ����ٸ� �ڽİ��� ���� ����
	if((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	//������ shared memory�� �ڽ��� virtual address�� attach
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

	//printf ��� ��� shared memory�� virtual address���� stack�� heap ���� ��ġ�� attach �Ǵ� ���� Ȯ�� ����
	

	//virtaul address���� shared memory detach
	if(shmdt(shmptr) < 0){
		perror("shmdt");
		exit(1);
	}
	//shared memory ����
	if(shmctl(shmid, IPC_RMID, 0) < 0){
		perror("shmctl");
		exit(1);
	}

	return 0;
}
