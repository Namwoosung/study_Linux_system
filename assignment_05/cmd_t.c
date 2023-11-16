#include <stdio.h>
#include <pthread.h>
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
	pthread_t tid;

	while(1){
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);
		if(cmd[0] == 'q')
			break;
		//입력을 받으면 해당 입력을 처리하는 thread를 생성해서 처리
		if(pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0){
			perror("pthread_create");
			exit(1);
		}
//마찬가지로 해당 부분이 활성화되면 cmd_i.c와 동일하게 동작
#if 0
		pthread_join(tid, NULL);
#endif
	}

	return 0;
}
