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

//�ڽ��� socket id�� ���������� ����ϴ� ������ signal handler���� ����ؾ� �ϱ⿡
int Sockfd;

//server�� ��� �������̴ٰ� ctrl + C�� �ԷµǸ� ����
//=> �Ʒ� �Լ��� SIGINT handler�� ����ؼ� ���
void CloseServer(){
	close(Sockfd);
	printf("\nTCP server exit.....\n");

	exit(0);
}

int main(int argc, char *argv[]){
	int newSockfd, cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;

	signal(SIGINT, CloseServer); //handler ���

	if((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){ //IPv4 �������� TCP�� socket ����
		perror("socket");
		exit(0);
	}

	//servAddr ���� ����
	bzero((char *)&servAddr, sizeof(servAddr)); //0���� �ʱ�ȭ
	servAddr.sin_family = PF_INET; //IPv4 ���
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP address ���� <- �ڽ��� IP address�� ������ �ϰ�� � ������ ������ ó���ϰڴ� = INADDR_ANY
	servAddr.sin_port = htons(SERV_TCP_PORT); //port number ���� <- tcp.h�� define

	//���� socket�� bind, sock descriptor�� �ڽ��� socket �ּҸ� �Ѱ���
	if(bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){ 
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); //�ǹ̾���

	printf("TCP server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while(1){
		newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen); //TCP connection�� ��ٸ�
		//cliAddr�� ����� ���ο� socket�� descriptor�� ��ȯ
		if(newSockfd < 0){
			perror("accept");
			exit(1);
		}		

		//����� socket���� read, write
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
	//���� TCP connection������ ���� ���� �������� ����
	//���� TCP connection�� �ξ����� thread Ȥ�� child process�� ������ ������ ó���ϰ� �ϰ�, �ڽ��� �ٷ� �ٸ� TCP connection�� ��ٸ����� �ؾ���

	return 0;
}
