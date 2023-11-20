#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(long)

int update(long *ptr){
	return ((*ptr)++);
}

int main(){
	int fd, i, counter;
	pid_t pid;
	caddr_t area;
	
	// /dev/zero�� open
	// zero file�� unix�� �����ϴ� Ư���� ����, ���⿡ write�ϸ� null�� ä������, ���⼭ read�ϸ� �� ��ŭ null�� ��ȯ����
	// ����ϰ� /dev/null file�� ����, null file�� ������ �ƿ� ������� ��
	if((fd = open("/dev/zero", O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	// zero file�� mapping�ؼ� area pointer�� ��ȯ
	if((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	// ������ zero file�� ���� read, write�� �ϴ� ���� �ƴϱ⿡ close
	close(fd);
	
	//parent�� child�� ����ȭ�� ���� pipe�� ���� -> pipe�� ���� ����ȭ ȿ���� ����
	//���� ���������� shared memory�� �����ؼ�, �ش� ��ġ�� long type ������ ����
	//parent�� child�� �����ư��� ������ +1 �ϸ� shared memory�� �����ϴ� ����
	TELL_WAIT();
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if (pid > 0){
		//parent process
		for(i = 0; i < NLOOPS; i += 2){
			//�ʱ� area ��ġ���� 0�� ���� -> counter�� 0�� �� ��
			//���� child�� +1�� �ϰ� �ٽ� parent�� ����, ���� ���࿡�� counter�� 2���� ��
			//�� counter�� i�� ������ ������ ������ ���̰�, �׷��� ������ mismatch�̹Ƿ� ����
			if((counter = update((long *)area)) != i){
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			//�ڽ��� �� ���� �� ������ child�� awake
			TELL_CHILD();
			//�ڽ��� child�� ������ ���� �� ���� ���
			WAIT_CHILD();
		}
	}else{
		//child process
		for(i = 1; i < NLOOPS; i += 2){
			//parent�� ������ ���
			WAIT_PARENT();
			//�����ϰ� area�� ������ counter�� �ް� area�� ������ +1
			if((counter = update((long *) area)) != i){
				fprintf(stderr, "Counter mismach\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			//�ڽ��� ������ �������� parent�� awake
			TELL_PARENT();
		}
	}

	return 0;
}
