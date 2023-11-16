#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
//signal handler�� ����� �Լ�
void SigUsrHandler(int signo){
	//signal�� SIGUSR1�� ���
	if (signo == SIGUSR1){
		printf("Received a SIGUSR1 signal\n");
	}
	//signal�� SIGUSR2�� ���
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
	//SIGUSR1�� SIGUSR2�� handler�� �Լ� ���
	//�ش� process�� signal�� ������ ���� �ٸ� terminal���� pidȭ�� �� kill command�� ����
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
