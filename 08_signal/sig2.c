#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
//signal handler로 등록할 함수
void SigUsrHandler(int signo){
	//signal이 SIGUSR1일 경우
	if (signo == SIGUSR1){
		printf("Received a SIGUSR1 signal\n");
	}
	//signal이 SIGUSR2일 경우
	else if(signo == SIGUSR2){
		printf("Received a SIGUSR2 signal\n");
	}
	else{
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

int main(){
	//SIGUSR1과 SIGUSR2의 handler로 함수 등록
	//해당 process에 signal을 보내기 위해 다른 terminal에서 pid화인 후 kill command로 전송
	if(signal(SIGUSR1, SigUsrHandler) == SIG_ERR){
		perror("signal");
		exit(1);
	}
	if(signal(SIGUSR2, SigUsrHandler) == SIG_ERR){
		perror("signal");
		exit(1);
	}

	for( ; ; )
		pause();

	return 0;
}
