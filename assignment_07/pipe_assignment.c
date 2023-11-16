#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 128

int main(){
	//fd1은 부모가 자식에게 전달, fd2는 자식이 부모에게 전달하는 용도
	int n, fd1[2], fd2[2];
	pid_t pid;
	char buf[MAX_BUF];

	if(pipe(fd1) < 0){
		perror("pipe");
		exit(1);
	}
	if(pipe(fd2) < 0){
		perror("pipe");
		exit(1);
	}
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(fd1[1]);
		close(fd2[0]);
		//child가 parent에게 data를 받는 부분
		printf("child : Wait for parent to send data\n");

		if((n = read(fd1[0], buf, MAX_BUF)) < 0){
			perror("read");
			exit(1);
		}

		printf("Child : Received data form parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
		//child가 parnet에게 data를 보내는 부분
		printf("Child: send data to parent\n");
		strcpy(buf, "Hello, World! from child\n");
		write(fd2[1], buf, strlen(buf) + 1);

	}else{
		close(fd1[0]);
		close(fd2[1]);
		//parent가 child에게 data를 보내는 부분
		strcpy(buf, "Hello, World! from parent\n");
		printf("Parent: Send data to child\n");

		write(fd1[1], buf, strlen(buf) + 1);
		
		//parent가 child에게 data를 받는 부분
		printf("Parent: Wait for parent to send data\n");

		if((n = read(fd2[0], buf, MAX_BUF)) < 0){
			perror("read");
			exit(1);
		}
		
		printf("parent: Received data form parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);

	}

	exit(0);

	return 0;
}
