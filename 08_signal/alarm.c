#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static unsigned int AlarmSecs; //Alarmsecs ������ SetPeriodicAlarm���� �޾� SigAlarmHandler������ ����ؾ� �ϱ⿡ ���������� ����
//���������� ����� ��� �ش� ���Ͽ����� ���� �� �ֵ��� static ������ ���� �ʿ�

void SigAlarmHandler(int signo){
	//������ signal hanlder�� �ѹ� ȣ��Ǹ� ����� �����Ǿ��⿡ ��� ����ϴ� ����
	//������������ �ѹ� ��ϵǸ� �������� �ʱ⿡ �ʿ� ���� ����
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		perror("signal");
		exit(1);
	}
	
	alarm(AlarmSecs); //�ֱ������� �˶� �߻�

	printf(".");
	fflush(stdout); //stdout�� ���ۿ� \n�� �� ������ �׾Ƶξ��ٰ� ��� .�� ����ϱ� ���� ���۸� flush����� ��

	return ;
}

int SetPeriodicAlarm(unsigned int nsecs){
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		return -1;
	}

	AlarmSecs = nsecs;	//AlarmSecs ����

	alarm(AlarmSecs);

	return 0;
}

int main(){
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1); //1�ʸ��� �˶��� �︮���� ����

	for( ; ; )
		pause();

	return 0;
}
