#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> //close function
#include <string.h> //bzero function
#include "udp.h"

//자신의 socket id를 전역변수로 사용하는 이유는 signal handler에서 사용해야 하기에
int Sockfd;

//server는 계속 수행중이다가 ctrl + C가 입력되면 종료
//=> 아래 함수를 SIGINT handler로 등록해서 사용
void CloseServer(){
	close(Sockfd);
	printf("\nUDP server exit.....\n");

	exit(0);
}

int main(int argc, char *argv[]){
	int cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;

	signal(SIGINT, CloseServer); //handler 등록

	if((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){ //IPv4 버전으로 UDP용 socket 생성
		perror("socket");
		exit(0);
	}

	//servAddr 지정 과정
	bzero((char *)&servAddr, sizeof(servAddr)); //0으로 초기화
	servAddr.sin_family = PF_INET; //IPv4 사용
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP address 지정 <- 자신의 IP address가 여러개 일경우 어떤 곳으로 오더라도 처리하겠다 = INADDR_ANY
	servAddr.sin_port = htons(SERV_UDP_PORT); //port number 지정 <- udp.h에 define

	//만든 socket을 bind, sock descriptor와 자신의 socket 주소를 넘겨줌
	if(bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){ 
		perror("bind");
		exit(1);
	}

	printf("UDP server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while(1){
		//UDP는 connectionless -> 바로 recvfrom으로 대기
		//Sockfd를 통해 msg를 받음, 여기서 client의 주소는 cliAddr에 저장되어 반환됨
		if((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0){
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);
		
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//cliAddr로 msg를 보냄
		if(sendto(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0){
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
	return 0;
}
