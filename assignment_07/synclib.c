#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static int Pfd1[2], Pfd2[2];

void TELL_WAIT(void){
	//pipe ���� �Լ�
	//2���� pipe�� ����
	//Pfd1�� parent�� child���� ������ pipe, Pfd2�� child�� parent���� ������ �뵵
	if(pipe(Pfd1) < 0 || pipe(Pfd2) < 0){
		perror("pipe");
		exit(1);
	}
}

void TELL_PARENT(void){
	//�θ𿡰� write�� ���� read���� ��� �� �ֵ��� ��
	if(write(Pfd2[1], "c", 1) != 1){
		perror("write");
		exit(1);
	}
}

void WAIT_PARENT(void){
	char c;	
	//parent�� write�� ��ٸ�
	if(read(Pfd1[0], &c, 1) != 1){
		perror("read");
		exit(1);
	}
	if(c != 'p'){
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

void TELL_CHILD(void){
	//child���� write�� ���� read���� ��� �� �ֵ��� ��
	if(write(Pfd1[1], "p", 1) != 1){
		perror("write");
		exit(1);
	}
}

void WAIT_CHILD(void){
	char c;
	//child�� write�� ��ٸ�
	if(read(Pfd2[0], &c, 1) != 1){
		perror("read");
		exit(1);
	}
	if(c != 'c'){
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
