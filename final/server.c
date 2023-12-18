#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <pthread.h>
#include "tcp.h"

int Sockfd;

void CloseServer(){
	close(Sockfd);
	printf("\nTCP server exit.....\n");

	exit(0);
}

void ProcessClient(int sockfd){
	int num, result = 0;
	int n, i;
	if((n = read(sockfd, (int *)&num, sizeof(num))) <0 ){
		perror("read");
		exit(1);
	}
	for(i = 0; i <= num; i++){
		result += num;
	}

	if(write(sockfd, (int *)&result, sizeof(num)) < 0){
		perror("write");
		exit(1);
	}
	
	close(sockfd);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	pthread_t tid;

	signal(SIGINT, CloseServer);

	if((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(0);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); 
	servAddr.sin_family = PF_INET; 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servAddr.sin_port = htons(SERV_TCP_PORT); 

	if(bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){ 
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); 

	printf("TCP server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while(1){
		newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
		if(newSockfd < 0){
			perror("accept");
			exit(1);
		}		
	
		if(pthread_create(&tid, NULL, (void *)ProcessClient, (void *)newSockfd) < 0){
			perror("pthread_create");
			exit(1);
		}
	}
	return 0;
}
