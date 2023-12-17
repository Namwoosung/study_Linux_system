#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> //for isdigit function
#include "tcp.h"

int main(int argc, char *argv[]){
	int sockfd, n;
	struct sockaddr_in servAddr;
	MsgType msg;
	struct hostent *hp;

	if( argc != 2){
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}

	//socket 생성
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("sockfd");
		exit(1);
	}

	//자신과 connect할 socket 주소를 지정
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);
	
	//ip address는 domain을 통해서도 얻을 수 있도록 변경
	if(isdigit(argv[1][0])){ //dotted decimal이면 그대로 사용
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	} else{ //domain일 경우
		if((hp = gethostbyname(argv[1])) == NULL){
			fprintf(stderr, "Unkown host: %s\n", argv[1]);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	//connect할 socket을 넘겨줘 TCP connection 수행
	if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("connect");
		exit(1);
	}

	//read, write 과정
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if(write(sockfd, (char *)&msg, sizeof(msg)) < 0){
		perror("write");
		exit(1);
	}
	printf("Sent a request....");

	if((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0){
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);

	return 0;
}
