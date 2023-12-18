#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 
#include "tcp.h"

int main(int argc, char *argv[]){
	int sockfd, n;
	struct sockaddr_in servAddr;
	struct hostent *hp;

	if( argc != 3){
		fprintf(stderr, "Usage: %s IPaddress number\n", argv[0]);
		exit(1);
	}

	int num = atoi(argv[2]);
	int result = 0;

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("sockfd");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);
	
	if(isdigit(argv[1][0])){ 
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	} else{
		if((hp = gethostbyname(argv[1])) == NULL){
			fprintf(stderr, "Unkown host: %s\n", argv[1]);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
		perror("connect");
		exit(1);
	}

	if(write(sockfd, (int *)&num, sizeof(num)) < 0){
		perror("write");
		exit(1);
	}

	if((n = read(sockfd, (int *)&result, sizeof(result))) < 0){
		perror("read");
		exit(1);
	}
	printf("%d\n",result);

	close(sockfd);

	return 0;
}
