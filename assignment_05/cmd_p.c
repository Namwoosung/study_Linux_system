#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CMD 256

void DoCmd(char *cmd){
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main(){
	char cmd[MAX_CMD];
	int pid;
	while(1){
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);
		if(cmd[0] == 'q')
			break;
		//입력을 받으면 fork로 새로운 process 생성
		if((pid = fork()) < 0){
			perror("fork");
			exit(1);
		}
		//child process에서 cmd를 처리, parent는 바로 대기하도록 동작
		else if(pid == 0){
			DoCmd(cmd);
			exit(0);
		}
//현재 if 0이므로 compile되지 않을 것, 이 부분을 활성화 시키면 parent는 child가 종료될 때 까지, 기다리도록 동작하기에, cmd_i.c와 동일하게 동작할 것
#if 0
		else{
			wait(NULL);
		}	
#endif
	}
	return 0;
}
