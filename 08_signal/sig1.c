#include <stdio.h>
#include <signal.h>
#include <stdlib.h> // for exit function
#include <unistd.h> // for pause function

//signal handler�� ����� �Լ�
void SigIntHandler(int signo){
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");
	
	exit(0);
}

int main(){
	signal(SIGINT, SigIntHandler); //SIGINT signal�� ������ SigIntHandler�� �����϶�� ���

	printf("Press ^C to quit\n");
	
	for( ; ;)
		pause(); //signal�� �� ������ ���

	return 0;
}
