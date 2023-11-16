#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static int Pfd1[2], Pfd2[2];

void TELL_WAIT(void){
	//pipe 생성 함수
	//2개의 pipe를 생성
	//Pfd1은 parent가 child에게 보내는 pipe, Pfd2는 child가 parent에게 보내는 용도
	if(pipe(Pfd1) < 0 || pipe(Pfd2) < 0){
		perror("pipe");
		exit(1);
	}
}

void TELL_PARENT(void){
	//부모에게 write를 보내 read에서 깨어날 수 있도록 함
	if(write(Pfd2[1], "c", 1) != 1){
		perror("write");
		exit(1);
	}
}

void WAIT_PARENT(void){
	char c;	
	//parent의 write를 기다림
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
	//child에게 write를 보내 read에서 깨어날 수 있도록 함
	if(write(Pfd1[1], "p", 1) != 1){
		perror("write");
		exit(1);
	}
}

void WAIT_CHILD(void){
	char c;
	//child의 write를 기다림
	if(read(Pfd2[0], &c, 1) != 1){
		perror("read");
		exit(1);
	}
	if(c != 'c'){
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
