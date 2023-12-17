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

//�ڽ��� socket id�� ���������� ����ϴ� ������ signal handler���� ����ؾ� �ϱ⿡
int Sockfd;

//server�� ��� �������̴ٰ� ctrl + C�� �ԷµǸ� ����
//=> �Ʒ� �Լ��� SIGINT handler�� ����ؼ� ���
void CloseServer(){
	close(Sockfd);
	printf("\nUDP server exit.....\n");

	exit(0);
}

int main(int argc, char *argv[]){
	int cliAddrLen, n;
	struct sockaddr_in cliAddr, servAddr;
	MsgType msg;

	signal(SIGINT, CloseServer); //handler ���

	if((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){ //IPv4 �������� UDP�� socket ����
		perror("socket");
		exit(0);
	}

	//servAddr ���� ����
	bzero((char *)&servAddr, sizeof(servAddr)); //0���� �ʱ�ȭ
	servAddr.sin_family = PF_INET; //IPv4 ���
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP address ���� <- �ڽ��� IP address�� ������ �ϰ�� � ������ ������ ó���ϰڴ� = INADDR_ANY
	servAddr.sin_port = htons(SERV_UDP_PORT); //port number ���� <- udp.h�� define

	//���� socket�� bind, sock descriptor�� �ڽ��� socket �ּҸ� �Ѱ���
	if(bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){ 
		perror("bind");
		exit(1);
	}

	printf("UDP server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while(1){
		//UDP�� connectionless -> �ٷ� recvfrom���� ���
		//Sockfd�� ���� msg�� ����, ���⼭ client�� �ּҴ� cliAddr�� ����Ǿ� ��ȯ��
		if((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0){
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);
		
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//cliAddr�� msg�� ����
		if(sendto(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0){
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
	return 0;
}
