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

	//socket ����
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("sockfd");
		exit(1);
	}

	//�ڽŰ� connect�� socket �ּҸ� ����
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);
	
	//ip address�� domain�� ���ؼ��� ���� �� �ֵ��� ����
	if(isdigit(argv[1][0])){ //dotted decimal�̸� �״�� ���
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	} else{ //domain�� ���
		if((hp = gethostbyname(argv[1])) == NULL){
			fprintf(stderr, "Unkown host: %s\n", argv[1]);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	//connect�� socket�� �Ѱ��� TCP connection ����
	if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("connect");
		exit(1);
	}

	//read, write ����
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