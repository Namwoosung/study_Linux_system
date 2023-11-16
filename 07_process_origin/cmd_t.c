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
		//�Է��� ������ �ش� �Է��� ó���ϴ� thread�� �����ؼ� ó��
		if(pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0){
			perror("pthread_create");
			exit(1);
		}
//���������� �ش� �κ��� Ȱ��ȭ�Ǹ� cmd_i.c�� �����ϰ� ����
#if 0
		pthread_join(tid, NULL);
#endif
	}

	return 0;
}
