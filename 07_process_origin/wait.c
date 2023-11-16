#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	int status;
	
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		printf("I'm a child\n");
		sleep(2);
	}
	else{
		//parent process�� �ڽ��� child process�� ����Ǳ⸦ wait
		//status�� child process�� exit�� status�� �Ҵ��
		wait(&status);
		//wait�� ����Ǹ� child�� ����� ���̱⿡ �ش� ���� ���
		printf("A child kelled with %d status\n", status);
	}

	return 0;
}
