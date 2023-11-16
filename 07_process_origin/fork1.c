#include <stdio.h> 
#include <sys/types.h> //for typedef
#include <unistd.h> //for fork function
#include <stdlib.h>

int main(){
	int pid;
	//fork error �� -1�� ��ȯ
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){ //pid�� 0�̸� child process
		/*child*/
		printf("I am %d. My parent is %d.\n", getpid(), getppid()); //�ڽ��� PID�� parent�� PID
	}
	else{ //pid�� child process�� parent process
		/*parent*/
		printf("I am %d. My child is %d.\n", getpid(), pid); //�ڽ��� PID�� child�� PID
	}

	return 0;
}
