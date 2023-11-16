#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	int status;
	
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		printf("I'm a child\n");
		sleep(2);
	}
	else{
		//parent process는 자신의 child process가 종료되기를 wait
		//status에 child process가 exit의 status가 할당됨
		wait(&status);
		//wait이 종료되면 child가 종료된 것이기에 해당 문구 출력
		printf("A child kelled with %d status\n", status);
	}

	return 0;
}
