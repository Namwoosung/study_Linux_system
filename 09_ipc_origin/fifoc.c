#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "fifo.h"

int main(){
	char fname[MAX_FIFO_NAME];
	int fd, sfd, n;
	MsgType msg;
	//client는 server로 부터 data를 받을 fifo file name을 설정
	sprintf(fname, ".fifo%d", getpid());
	//fifo file 생성
	if(mkfifo(fname, 0600) < 0){
		perror("mkfifio");
		exit(1);
	}
	//내가 받을 fifo file과 보낼 server의 fifo file 모두 open
	if((fd = open(fname, O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	if((sfd = open(SERV_FIFO, O_RDWR)) < 0){
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request from %d.", getpid());
	//msg설정 후 server가 기다리는 fifo file에 write
	write(sfd, (char *)&msg, sizeof(msg));
	printf("Sent a request.....");
	//server가 자신의 fifo에 reply를 할 때까지 대기 후 read
	if((n = read(fd, (char *)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	close(fd);
	close(sfd);
	//종료하기 전 자신이 만든 fifo file은 remove
	if(remove(fname) < 0){
		perror("remove");
		exit(1);
	}
	return 0;
}
