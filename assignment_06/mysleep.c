#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//signal handler �Լ�, �ƹ��͵� ���۵� ����
void SigAlarmHandler(int signo){
	return ;
}

unsigned int mysleep(unsigned int nsecs){
	//signal handler ���
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		return nsecs;
	}
	//nsecs ���� alarm signal�� ���޵ǵ��� ����
	alarm(nsecs);
	//signal �߻� ���� ���
	pause();

	return alarm(0); //alarm(0)�� ������ �� alarm�� ���� �ð��� return
}

int main(){
	printf("Wait for 5 seconds ..\n");
	
	mysleep(5);

	return 0;
}
