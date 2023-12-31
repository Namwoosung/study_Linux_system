#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "select.h"

int TcpSockfd;
int UdpSockfd;
int UcoSockfd;
int UclSockfd;

//SIGINT handler로 등록할 함수
void CloseServer(){
	close(TcpSockfd);
	close(UdpSockfd);
	close(UcoSockfd);
	close(UclSockfd);
	if(remove(UNIX_STR_PATH) < 0){
		perror("remove");
	}
	if(remove(UNIX_DG_PATH) < 0){
		perror("remove");
	}

	printf("\nServer daemon exit....\n");
	
	exit(0);
}

//각각의 socket 생성
void MakeTcpSocket(){
	struct sockaddr_in servAddr;

	if((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if(bind(TcpSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5);
}

void MakeUdpSocket(){
	struct sockaddr_in servAddr;

	if((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if(bind(UdpSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("bind");
		exit(1);
	}
}

void MakeUcoSocket(){
	struct sockaddr_un servAddr;
	int servAddrLen;

	if((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_STR_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if(bind(UcoSockfd, (struct sockaddr *)&servAddr, servAddrLen) < 0){
		perror("bind");
		exit(1);
	}

	listen(UcoSockfd, 5);
}

void MakeUclSocket(){
	struct sockaddr_un servAddr;
	int servAddrLen;

	if((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	if(bind(UclSockfd, (struct sockaddr *)&servAddr, servAddrLen) < 0){
		perror("bind");
		exit(1);
	}
}

//socket마다 요청이 오면 처리
void ProcessTcpRequest(void *dummy){
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr;
	MsgType msg;

	cliAddrLen = sizeof(cliAddr);
	newSockfd = accept(TcpSockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
	if(newSockfd < 0){
		perror("accept");
		exit(1);
	}

	if((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s....", msg.data);
	
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if(write(newSockfd, (char *)&msg, sizeof(msg)) < 0){
		perror("write");
		exit(1);
	}
	printf("Replied.\n");
	
	close(newSockfd);
	pthread_exit(NULL);
}

void ProcessUdpRequest(void *dummy){
	int cliAddrLen, n;
	struct sockaddr_in cliAddr;
	MsgType msg;

	cliAddrLen = sizeof(cliAddr);
	if((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0){
		perror("reavfrom");
		exit(1);
	}
	printf("Received UDP request: %s....", msg.data);
	
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if(sendto(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0){
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
	pthread_exit(NULL);
}

void ProcessUcoRequest(void *dummy){
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr;
	MsgType msg;

	cliAddrLen = sizeof(cliAddr);
	newSockfd = accept(UcoSockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
	if(newSockfd < 0){
		perror("accept");
		exit(1);
	}

	if((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}
	printf("Received UNIX-doamin CO  request: %s....", msg.data);
	
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if(write(newSockfd, (char *)&msg, sizeof(msg)) < 0){
		perror("write");
		exit(1);
	}
	printf("Replied.\n");
	
	close(newSockfd);
	pthread_exit(NULL);
}

void ProcessUclRequest(void *dummy){
	int cliAddrLen, n;
	struct sockaddr_in cliAddr;
	MsgType msg;

	cliAddrLen = sizeof(cliAddr);
	if((n = recvfrom(UclSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0){
		perror("reavfrom");
		exit(1);
	}
	printf("Received UNIX-domain CL request: %s....", msg.data);
	
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if(sendto(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0){
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	fd_set fdvar;
	int count;
	pthread_t tid;

	signal(SIGINT, CloseServer);
	
	//socket 생성
	MakeTcpSocket();
	MakeUdpSocket();
	MakeUcoSocket();
	MakeUclSocket();

	printf("Server daemon started....\n");
	
	while(1){
		//내가 기다릴 socket을 fdvar에 설정
		FD_ZERO(&fdvar);
		FD_SET(TcpSockfd, &fdvar);
		FD_SET(UdpSockfd, &fdvar);
		FD_SET(UcoSockfd, &fdvar);
		FD_SET(UclSockfd, &fdvar);
		//read 변수만 선언해서 넘겨줌
		if((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL)) < 0){
			perror("select");
			exit(1);
		}
		//socket이 온 개수만큼 socket마다 함수를 불러서 처리
		while(count--){
			//FD_ISSET으로 어떤 socket으로 왔는지 check
			if(FD_ISSET(TcpSockfd, &fdvar)){
				if(pthread_create(&tid, NULL, (void *)ProcessTcpRequest, (void *)NULL) < 0){
					perror("pthread_create");
					exit(1);
				}
			}
			else if(FD_ISSET(UdpSockfd, &fdvar)){
				if(pthread_create(&tid, NULL, (void *)ProcessUdpRequest, (void *)NULL) < 0){
					perror("pthread_create");
					exit(1);
				}
			}
			else if(FD_ISSET(UcoSockfd, &fdvar)){
				if(pthread_create(&tid, NULL, (void *)ProcessUcoRequest, (void *)NULL) < 0){
					perror("pthread_create");
					exit(1);
				}
			}
			else if(FD_ISSET(UclSockfd, &fdvar)){
				if(pthread_create(&tid, NULL, (void *)ProcessUclRequest, (void *)NULL) < 0){
					perror("pthread_create");
					exit(1);
				}
			}
		}
	}
	return 0;
}
