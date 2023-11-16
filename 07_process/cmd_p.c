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
		//�Է��� ������ fork�� ���ο� process ����
		if((pid = fork()) < 0){
			perror("fork");
			exit(1);
		}
		//child process���� cmd�� ó��, parent�� �ٷ� ����ϵ��� ����
		else if(pid == 0){
			DoCmd(cmd);
			exit(0);
		}
//���� if 0�̹Ƿ� compile���� ���� ��, �� �κ��� Ȱ��ȭ ��Ű�� parent�� child�� ����� �� ����, ��ٸ����� �����ϱ⿡, cmd_i.c�� �����ϰ� ������ ��
#if 0
		else{
			wait(NULL);
		}	
#endif
	}
	return 0;
}
