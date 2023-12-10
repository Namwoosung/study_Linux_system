#include <stdio.h>
#include <signal.h>

//signal handler로 등록할 함수
void SigIntHandler(int signo){
	printf("Received a SIGINT signal\n");
	exit(0);
}

int main(){
	//siganl handler 등록 방법
	if(signal(SIGINT, SigIntHandler) == SIG_ERR){
		perror("signal");
		exit(1);
	}

	for(; ;)
		pause(); //signal이 올 때가지 wait
}



//alarm system call을 이용해 sleep함수 구현, 주기적인 알람을 발생시키는 코드는 assignment_06/alarm.c 참조
unsigned int mysleep(unsigned int nsecs){
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		return nsecs;
	}
	
	alarm(nsecs); //nsecs 이후 SIGALRM signal을 발생시키는 system call

	pause();

	return alarm(0);
}



//signal handler는 어떤 thread에서 등록하던지, main thread에서 signal을 받아서 동작을 수행시킴


//pthread_cancel로 다른 곳에서 thread를 종료 시킬 수 있음 <- 종료 관련 설정 함수 2개 존재 <= assignment_06/cancel.c 참조


