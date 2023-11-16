#include <stdio.h> 
#include <sys/types.h> //for typedef
#include <unistd.h> //for fork function
#include <stdlib.h>

int main(){
	int pid;
	//fork error 시 -1을 반환
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){ //pid가 0이면 child process
		/*child*/
		printf("I am %d. My parent is %d.\n", getpid(), getppid()); //자신의 PID와 parent의 PID
	}
	else{ //pid가 child process면 parent process
		/*parent*/
		printf("I am %d. My child is %d.\n", getpid(), pid); //자신의 PID와 child의 PID
	}

	return 0;
}
