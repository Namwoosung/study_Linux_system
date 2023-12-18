#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int count;

void SigChldHandler(int signo){
	printf("%d\n", count);
	exit(0);
}

int main(){
	int pid;
	count = 0;

	if(signal(SIGCHLD, SigChldHandler) == SIG_ERR){
		perror("signal");
		exit(1);
	}

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	} else if(pid == 0){
		sleep(1);
	} else{
		while(1){
			count++;
		}
	}

	return 0;
}
