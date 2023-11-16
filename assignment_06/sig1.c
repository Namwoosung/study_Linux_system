#include <stdio.h>
#include <signal.h>
#include <stdlib.h> // for exit function
#include <unistd.h> // for pause function

//signal handler로 등록할 함수
void SigIntHandler(int signo){
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");
	
	exit(0);
}

int main(){
	signal(SIGINT, SigIntHandler); //SIGINT signal을 받으면 SigIntHandler를 수행하라고 등록

	printf("Press ^C to quit\n");
	
	for( ; ;)
		pause(); //signal이 올 때까지 대기

	return 0;
}
