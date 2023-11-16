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
	//client�� server�� ���� data�� ���� fifo file name�� ����
	sprintf(fname, ".fifo%d", getpid());
	//fifo file ����
	if(mkfifo(fname, 0600) < 0){
		perror("mkfifio");
		exit(1);
	}
	//���� ���� fifo file�� ���� server�� fifo file ��� open
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
	//msg���� �� server�� ��ٸ��� fifo file�� write
	write(sfd, (char *)&msg, sizeof(msg));
	printf("Sent a request.....");
	//server�� �ڽ��� fifo�� reply�� �� ������ ��� �� read
	if((n = read(fd, (char *)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	close(fd);
	close(sfd);
	//�����ϱ� �� �ڽ��� ���� fifo file�� remove
	if(remove(fname) < 0){
		perror("remove");
		exit(1);
	}
	return 0;
}
