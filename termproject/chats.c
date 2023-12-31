#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "chat.h"

#define DEBUG

#define MAX_CLIENT 5
#define MAX_ID 32
#define MAX_BUF 256

typedef struct {
	int sockfd; //client socket address
	int inUse; //활성화 여부
	char uid[MAX_ID]; //server에 입장할 때 설정하는 user id
} ClientType;

int Sockfd;

//최대 client만큼 배열 생성
ClientType Client[MAX_CLIENT];

//사용중이지 않은 clinet를 하나 사용중으로 바꾸고, 해당 index를 return
int GetID(){
	int i;
	
	for(i = 0; i < MAX_CLIENT; i++){
		if(!Client[i].inUse){
			Client[i].inUse = 1;
			return i;
		}
	}
}

//자신 이외에 다른 clinet들에게 message 전송
void SendToOtherClients(int id, char *buf){
	int i;
	char msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef DEBUG
	printf("%s", msg);
	fflush(stdout);
#endif

	for(i = 0; i < MAX_CLIENT; i++){
		if(Client[i].inUse && (i != id)){
			if(send(Client[i].sockfd, msg, strlen(msg) + 1, 0) < 0){
				perror("send");
				exit(1);
			}
		}
	}
}

void ProcessClient(int id){
	char buf[MAX_BUF];
	int n;

	
	//데이터를 받아 buf에 저장
	if((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0){
		perror("recv");
		exit(1);
	}
	if(n == 0){ //0이면 log-out이므로 socket을 닫고, 비활성화 해줌
		printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);

		close(Client[id].sockfd);
		Client[id].inUse = 0;

		strcpy(buf, "log-out....\n");
	}
	//buf를 다른 client들에게 전송
	SendToOtherClients(id, buf);
}

void CloseServer(int signo){
	int i;
	
	close(Sockfd);
	
	for(i = 0; i < MAX_CLIENT; i++){
		if(Client[i].inUse){ //활성화되어 있는 모든 client의 thread 종료 및 socket close
			close(Client[i].sockfd);
		}
	}

	printf("\nChat server terminated.....\n");

	exit(0);
}

int main(int argc, char *argv[]){
	int newSockfd, cliAddrLen, id, count, n, i, one = 1;
	struct sockaddr_in cliAddr, servAddr;
	fd_set fdvar;

	signal(SIGINT, CloseServer);

	if((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(1);
	}

	if(setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0){
		perror("setsockopt");
		exit(1);
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

	printf("Chat server started....\n");

	FD_ZERO(&fdvar);
	FD_SET(Sockfd, &fdvar);

	cliAddrLen = sizeof(cliAddr);
	while(1){
		if((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL)) < 0){
			perror("select");
			exit(1);
		}
		
		if(FD_ISSET(Sockfd, &fdvar)){
			newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
			if(newSockfd < 0){
				perror("accept");
				exit(1);
			}
			id = GetID();
			if(id == -1){
				close(newSockfd);
				continue;
			}
			Client[id].sockfd = newSockfd;
			FD_SET(newSockfd, &fdvar);
			//recv는 socket으로 부터 data를 받아오는 함수
			//해당 id의 socket으로 부터 user id정보를 받아 client[id].uid buffer에 저장
			if((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0){
				perror("recv");
				exit(1);
			}
			printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);
			ProcessClient(id);
		}else{
			for(i = 0; i < MAX_CLIENT; i++){
				if(Client[i].inUse && FD_ISSET(Client[id].sockfd, &fdvar)){
					ProcessClient(id);
				}
			}
		
		}
	}
	return 0;
}
