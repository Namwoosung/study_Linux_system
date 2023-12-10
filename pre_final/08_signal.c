#include <stdio.h>
#include <signal.h>

//signal handler�� ����� �Լ�
void SigIntHandler(int signo){
	printf("Received a SIGINT signal\n");
	exit(0);
}

int main(){
	//siganl handler ��� ���
	if(signal(SIGINT, SigIntHandler) == SIG_ERR){
		perror("signal");
		exit(1);
	}

	for(; ;)
		pause(); //signal�� �� ������ wait
}



//alarm system call�� �̿��� sleep�Լ� ����, �ֱ����� �˶��� �߻���Ű�� �ڵ�� assignment_06/alarm.c ����
unsigned int mysleep(unsigned int nsecs){
	if(signal(SIGALRM, SigAlarmHandler) == SIG_ERR){
		return nsecs;
	}
	
	alarm(nsecs); //nsecs ���� SIGALRM signal�� �߻���Ű�� system call

	pause();

	return alarm(0);
}



//signal handler�� � thread���� ����ϴ���, main thread���� signal�� �޾Ƽ� ������ �����Ŵ


//pthread_cancel�� �ٸ� ������ thread�� ���� ��ų �� ���� <- ���� ���� ���� �Լ� 2�� ���� <= assignment_06/cancel.c ����


