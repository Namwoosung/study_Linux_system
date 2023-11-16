#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//signal handler 함수, 아무것도 동작도 안함
void SigAlarmHandler(int signo){
	return ;
}

unsigned int mysleep(unsigned int nsecs){
	//signal handler 등록
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		return nsecs;
	}
	//nsecs 이후 alarm signal이 전달되도록 설정
	alarm(nsecs);
	//signal 발생 까지 대기
	pause();

	return alarm(0); //alarm(0)은 이전에 한 alarm의 남은 시간을 return
}

int main(){
	printf("Wait for 5 seconds ..\n");
	
	mysleep(5);

	return 0;
}
