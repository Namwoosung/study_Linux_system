#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "udp.h"

int main(int argc, char *args[]){
	int sockfd, n, peerAddrLen;
	struct sockaddr_in servAddr, peerAddr;
	MsgType msg;

	//socket 생성
	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
		perror("sockfd");
		exit(1);
	}

	//자신과 connect할 socket 주소를 지정
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	//read, write 과정
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if(sendto(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("sendto");
		exit(1);
	}
	printf("Sent a request....");

	peerAddrLen = sizeof(peerAddr);
	if((n = recvfrom(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0){
		perror("recvfrom");
		exit(1);
	}	

	printf("Received reply: %s\n", msg.data);

	close(sockfd);

	return 0;
}
