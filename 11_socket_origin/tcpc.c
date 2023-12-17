#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tcp.h"

int main(int argc, char *args[]){
	int sockfd, n;
	struct sockaddr_in servAddr;
	MsgType msg;

	//socket ����
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("sockfd");
		exit(1);
	}

	//�ڽŰ� connect�� socket �ּҸ� ����
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	servAddr.sin_port = htons(SERV_TCP_PORT);

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
