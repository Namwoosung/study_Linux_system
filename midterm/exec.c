#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t pid1, pid2;

	if((pid1 = fork()) < 0){
		perror("fork");
		exit(1);
	}else if(pid1 == 0){
		if(execl("/home/lsp15/midterm/print", "print", "HelloWorld!", "6", NULL) < 0){
			perror("execl");
			exit(1);
		}
	}

	char *argv[] = {"print", "HelloWorld!", "6", NULL};

	if((pid2 = fork()) < 0){
		perror("fork");
		exit(1);
	} else if(pid2 == 0){
		if(execv("/home/lsp15/midterm/print", argv) < 0){
			perror("execv");
			exit(1);
		}
	}

	wait(NULL);
	wait(NULL);
	
	return 0;
}
