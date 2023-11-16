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
	//fork 이전 pipe 생성, fd[0]는 read, fd[1]은 write 용도
	if(pipe(fd) < 0){
		perror("pipe");
		exit(1);
	}
	//fork를 수행, child와 parent는 동일한 fd[0], fd[]1을 가르킴
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		//child process
		//read만 할 것이기에 fd[1] close
		close(fd[1]);
		printf("child : Wait for parent to send data\n");
		//buf에 내용이 쓰여질 때 까지 대기, 만약 child process가 먼저 수행된다면 여기서 waiting queue에 들어가서 buf에 data가 올 때 까지 대기할 것
		if((n = read(fd[0], buf, MAX_BUF)) < 0){
			perror("read");
			exit(1);
		}
		printf("Child : Received data form parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
	}else{
		//parent process
		//read용 file descriptor close
		close(fd[0]);
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		//pipe에 data 전달
		write(fd[1], buf, strlen(buf) + 1);
	}

	//pipe는 message passing 방식이므로 synchronization을 운영체제가 관리해 줌

	exit(0);

	return 0;
}
