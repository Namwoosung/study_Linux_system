#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static unsigned int AlarmSecs; //Alarmsecs 변수는 SetPeriodicAlarm에서 받아 SigAlarmHandler에서도 사용해야 하기에 전역변수로 선언
//전역변수를 사용할 경우 해당 파일에서만 사용될 수 있도록 static 변수로 선언 필요

void SigAlarmHandler(int signo){
	//기존의 signal hanlder는 한번 호출되면 등록이 해제되었기에 계속 등록하는 과정
	//리눅스에서는 한번 등록되면 해제되지 않기에 필요 없는 과정
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		perror("signal");
		exit(1);
	}
	
	alarm(AlarmSecs); //주기적으로 알람 발생

	printf(".");
	fflush(stdout); //stdout은 버퍼에 \n이 올 때까지 쌓아두었다가 출력 .만 출력하기 위해 버퍼를 flush해줘야 함

	return ;
}

int SetPeriodicAlarm(unsigned int nsecs){
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		return -1;
	}

	AlarmSecs = nsecs;	//AlarmSecs 설정

	alarm(AlarmSecs);

	return 0;
}

int main(){
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1); //1초마다 알람이 울리도록 설정

	for( ; ; )
		pause();

	return 0;
}
