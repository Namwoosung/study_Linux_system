#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void SigChldHandler(int signo){
	//child process�� ����Ǹ� print ���
	printf("child is terminated\n");
	//asychronout�ϰ� parent�� �� ���� ����
	printf("Doing something, ..., terminate process after 1 second\n");
	sleep(1);
	//����
	exit(0);
}

int main(){
	pid_t pid;
	int status;
	
	signal(SIGCHLD, SigChldHandler);

	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		printf("I'm a child\n");
		sleep(2);
	}
	else{
		//parent process�� wihle ���� �ݺ�, �����δ� �����ؾ� �� ������ �Ѵٰ� ����
		while(1)
			;			
	}

	return 0;
}
