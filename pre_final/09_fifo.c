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

//fifo ���� �� Ȱ��
// 	fifo.h
#define MAX_FIFO_NAME 32
#define SERV_FIFO "./.fifo"

typedef struct{
	char returnFifo[MAX_FIFO_NAME];
	char data[128];
}	MsgType;



//	fifo server -> �ڽ��� ���� fifo�� �����, cilent���� request�� �ް�, �ش� clinet���� ���� data�� client fifo�� ����
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

	if(signal(SIGINT, SigIntHandler) == SIG_ERR){ //handler ��� <- ���� �� fifo�� ����� ����
		perror("signal");
		exit(1);
	}
	//fifo.h�� ������ �̸����� fifo ����
	if(mkfifo(SERV_FIFO, 0600) < 0){
		//EEXIST error�� file�� �̹� �����Ѵٴ� �ǹ��̹Ƿ� ������ ������ ����ó������ ����
		if (errno != EEXIST){
			perror("mkfifo");
			exit(1);
		}
	}
	//������ fifo file�� open
	if((fd = open(SERV_FIFO, O_RDWR)) < 0){
		perror("open");
		exit(1);
	}

	while(1) {
		//���� �ݺ��ϸ� client���� requet�� ���� �� ���� ���
		if((n = read(fd, (char *)&msg, sizeof(msg))) < 0){
			//read ��� �� signal �߻� �� EINTR error�� �߻��ϴµ� ������ ������ continue
			if(errno == EINTR){
				continue;
			}
			else{
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s....", msg.data);
		
		//clinet���� ���� msg���� clinet�� �Է��� ��ٸ��� fifo file name�� �޾� �ش� file�� open
		if((cfd = open(msg.returnFifo, O_WRONLY)) < 0){
			perror("open");
			exit(1);
		}
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//msg.data ������ �ٲٰ� �ش� ������ fifo�� ����
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

	sprintf(fname, ".fifo%d", getpid()); //�ڽ��� data�� ���� fifo�̸� ����
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

