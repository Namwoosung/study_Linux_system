#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> //close function
#include <string.h> //bzero function
#include "tcp.h"

//자신의 socket id를 전역변수로 사용하는 이유는 signal handler에서 사용해야 하기에
int Sockfd;

//server는 계속 수행중이다가 ctrl + C가 입력되면 종료
//=> 아래 함수를 SIGINT handler로 등록해서 사용
void CloseServer(){
	close(Sockfd);
	printf("\nTCP server exit.....\n");

	exit(0);
}

int main(int argc, char *argv[]){
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;

	signal(SIGINT, CloseServer); //handler 등록

	if((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){ //IPv4 버전으로 TCP용 socket 생성
		perror("socket");
		exit(0);
	}

	//servAddr 지정 과정
	bzero((char *)&servAddr, sizeof(servAddr)); //0으로 초기화
	servAddr.sin_family = PF_INET; //IPv4 사용
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP address 지정 <- 자신의 IP address가 여러개 일경우 어떤 곳으로 오더라도 처리하겠다 = INADDR_ANY
	servAddr.sin_port = htons(SERV_TCP_PORT); //port number 지정 <- tcp.h에 define

	//만든 socket을 bind, sock descriptor와 자신의 socket 주소를 넘겨줌
	if(bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){ 
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); //의미없음

	printf("TCP server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while(1){
		newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen); //TCP connection을 기다림
		//cliAddr과 연결된 새로운 socket의 descriptor를 반환
		if(newSockfd < 0){
			perror("accept");
			exit(1);
		}		

		//연결된 socket으로 read, write
		if((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0){
			perror("read");
			exit(1);
		}
		printf("Received request: %s....", msg.data);
		
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if(write(newSockfd, (char *)&msg, sizeof(msg)) < 0){
			perror("write");
			exit(1);
		}
		printf("Relied.\n");

		usleep(10000);
		close(newSockfd);
	}
	//원래 TCP connection에서는 위와 같이 동작하지 않음
	//원래 TCP connection이 맺어지면 thread 혹은 child process를 생성해 동작을 처리하게 하고, 자신은 바로 다른 TCP connection을 기다리도록 해야함

	return 0;
}
