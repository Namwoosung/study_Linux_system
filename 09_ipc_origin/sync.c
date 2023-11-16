#include <stdio.h>
#include <stdlib.h>
#include "synclib.h"

#define NLOOPS 5

int main(){
	int i;
	pid_t pid;
	//pipe 2�� ����, ����� ����� ����
	TELL_WAIT();
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if(pid > 0){
		for(i = 0; i < NLOOPS; i++){
			/* To do sometion...*/
			//�� ������ ������ child�� awake
			TELL_CHILD();
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			//child�� �� ���� ��ĥ �� ���� wait
			WAIT_CHILD();
		}
	} else{
		for(i = 0; i < NLOOPS; i++){
			printf("child: Wait for parent to tell\n");
			//parent�� �� ���� ��ĥ �� ���� wait
			WAIT_PARENT();
			/*To do somehting...*/
			//�� ���� �� ������ parent�� awake
			TELL_PARENT();
			printf("Child: Tell to parent\n");
		}
	}

	return 0;
}
