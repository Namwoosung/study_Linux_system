#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void SigChldHandler(int signo){
	//child process가 종료되면 print 출력
	printf("child is terminated\n");
	//asychronout하게 parent는 할 일을 진행
	printf("Doing something, ..., terminate process after 1 second\n");
	sleep(1);
	//종료
	exit(0);
}

int main(){
	pid_t pid;
	int status;
	
	signal(SIGCHLD, SigChldHandler);

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		printf("I'm a child\n");
		sleep(2);
	}
	else{
		//parent process는 wihle 무한 반복, 실제로는 수행해야 할 동작을 한다고 가정
		while(1)
			;			
	}

	return 0;
}
