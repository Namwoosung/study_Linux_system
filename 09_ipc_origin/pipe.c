#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 128

int main(){
	int n, fd[2];
	pid_t pid;
	char buf[MAX_BUF];
	//fork ���� pipe ����, fd[0]�� read, fd[1]�� write �뵵
	if(pipe(fd) < 0){
		perror("pipe");
		exit(1);
	}
	//fork�� ����, child�� parent�� ������ fd[0], fd[]1�� ����Ŵ
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		//child process
		//read�� �� ���̱⿡ fd[1] close
		close(fd[1]);
		printf("child : Wait for parent to send data\n");
		//buf�� ������ ������ �� ���� ���, ���� child process�� ���� ����ȴٸ� ���⼭ waiting queue�� ���� buf�� data�� �� �� ���� ����� ��
		if((n = read(fd[0], buf, MAX_BUF)) < 0){
			perror("read");
			exit(1);
		}
		printf("Child : Received data form parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
	}else{
		//parent process
		//read�� file descriptor close
		close(fd[0]);
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		//pipe�� data ����
		write(fd[1], buf, strlen(buf) + 1);
	}

	//pipe�� message passing ����̹Ƿ� synchronization�� �ü���� ������ ��

	exit(0);

	return 0;
}
