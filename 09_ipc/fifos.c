#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "fifo.h"

//SIGINT를 받을 경우 만들어 놓았던 fifo를 삭제하고 종료
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
	//signal handler등록
	if(signal(SIGINT, SigIntHandler) == SIG_ERR){
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
