#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "fifo.h"

//fifo 생성 및 활용
// 	fifo.h
#define MAX_FIFO_NAME 32
#define SERV_FIFO "./.fifo"

typedef struct{
	char returnFifo[MAX_FIFO_NAME];
	char data[128];
}	MsgType;



//	fifo server -> 자신이 받을 fifo를 만들고, cilent에게 request를 받고, 해당 clinet에게 보낼 data를 client fifo로 전송
void SigIntHandler(int signo){
	if(remove(SERV_FIFO) < 0){
		perror("remove");
		exit(1);
	}

	exit(0);
}

int main(){
	int fd, cfd, n;
	MsgType msg;

	if(signal(SIGINT, SigIntHandler) == SIG_ERR){ //handler 등록 <- 종료 시 fifo를 지우는 역할
		perror("signal");
		exit(1);
	}
	//fifo.h에 선언한 이름으로 fifo 생성
	if(mkfifo(SERV_FIFO, 0600) < 0){
		//EEXIST error는 file이 이미 존재한다는 의미이므로 문제가 없으니 예외처리에서 제외
		if (errno != EEXIST){
			perror("mkfifo");
			exit(1);
		}
	}
	//생성한 fifo file을 open
	if((fd = open(SERV_FIFO, O_RDWR)) < 0){
		perror("open");
		exit(1);
	}

	while(1) {
		//무한 반복하며 client들이 requet를 보낼 때 까지 대기
		if((n = read(fd, (char *)&msg, sizeof(msg))) < 0){
			//read 대기 중 signal 발생 시 EINTR error가 발생하는데 문제가 없으니 continue
			if(errno == EINTR){
				continue;
			}
			else{
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s....", msg.data);
		
		//clinet에게 받은 msg에서 clinet가 입력을 기다리는 fifo file name을 받아 해당 file을 open
		if((cfd = open(msg.returnFifo, O_WRONLY)) < 0){
			perror("open");
			exit(1);
		}
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//msg.data 정보를 바꾸고 해당 정보를 fifo에 전달
		write(cfd, (char *)&msg, sizeof(msg));
		
		close(cfd);
		
		printf("Replied.\n");
	}
	return 0;
}



//	fifo client

int main(){
	char fname[MAX_FIFO_NAME];
	int fd, sfd, n;
	MsgType msg;

	sprintf(fname, ".fifo%d", getpid()); //자신이 data를 받을 fifo이름 생성
	if(mkfifo(fname, 0600) < 0){
		perror("mkfifo");
		exit(1);
	}
	
	if((fd = open(fname, O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	if(sfd = open(SERV_FIFO, O_RDWR) < 0){
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request form %d.", getpid());
	write(sfd, (char *)&msg, sizeof(msg));

	if((n = read(fd, (char *)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}

	close(fd); close(sfd);

	if(remove(fname) < 0){
		perror("fname");
		exit(1);
	}

	return 0;
}

