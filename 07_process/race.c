#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void CharAtaTime(char *str){
	char *ptr;
	int c, i;
	
	setbuf(stdout, NULL); //stdout의 buffer를 NULL로 설정해 버퍼링되지 않고, 바로 출력하게 됨
	for(ptr = str; c = *ptr++;){
		//반복을 하는 이유는 race condition을 명시해 주기 위해 time quantum을 소진하도록 하는 것
		//child와 parent process는 문자 하나를 출력하고, 반복문을 돌다가 time qunatum을 소진하고, ready 상태로 돌아갈 것
		for(i = 0; i < 999999; i++)
			;
		//문자를 하나씩 출력
		putc(c, stdout);
	}
}

int main(){
	pid_t pid;
	
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	//child process와 parent process는 각각 CharAtaTime 함수를 시행
	//함수 동작에 따라 문자를 출력하다가 time quantum을 모두 소진해 ready로 돌아가기를 반복할 것
	//console이 shared data가 되어 번갈아 가며 출력될 것
	else if(pid == 0){
		CharAtaTime("output from child\n");
	}
	else{
		CharAtaTime("output from parent\n");
	}

	return 0;
}
