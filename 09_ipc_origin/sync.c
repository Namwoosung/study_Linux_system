#include <stdio.h>
#include <stdlib.h>
#include "synclib.h"

#define NLOOPS 5

int main(){
	int i;
	pid_t pid;
	//pipe 2개 생성, 양방향 통신을 위해
	TELL_WAIT();
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if(pid > 0){
		for(i = 0; i < NLOOPS; i++){
			/* To do sometion...*/
			//할 동작을 끝내면 child를 awake
			TELL_CHILD();
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			//child가 할 일을 마칠 때 까지 wait
			WAIT_CHILD();
		}
	} else{
		for(i = 0; i < NLOOPS; i++){
			printf("child: Wait for parent to tell\n");
			//parent가 할 일을 마칠 때 까지 wait
			WAIT_PARENT();
			/*To do somehting...*/
			//할 일을 다 했으면 parent를 awake
			TELL_PARENT();
			printf("Child: Tell to parent\n");
		}
	}

	return 0;
}
