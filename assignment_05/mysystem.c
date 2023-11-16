#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int mysystem(char *cmd){
	pid_t pid;
	int status;	
	
	//execv에 파라미터로 넘겨줄 argv array
	//system 함수는 /bin/sh -c command를 호출해 명령어를 수행, 이후 shell의 종료 status를 반환하도록 동작
	char* argv[] = {"/bin/sh", "-c", cmd, NULL};
	//fork 실패 시 error 및 -1 반환
	if((pid = fork()) < 0){
		perror("fork");
		return -1;
	}
	else if(pid == 0){
		//cmd command 실행, 실패 시 error 및 종료
		if(execv("/bin/sh", argv) < 0){
			perror("execv");
			exit(1);
		}
	}
	//parent process는 child가 끝나기를 기다리도록 동작
	if(waitpid(pid, &status, 0) < 0){
		perror("waitpid");
		return -1;
	}
	//명령어의 exit code를 알려주는 function
	return WEXITSTATUS(status);
}

//main 함수에서 mysystem함수 사용 예시
//system.c 실습 file과 동일한 command들을 수행
int main(){
	int status;

	if((status = mysystem("date")) < 0){
		perror("mysystem");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if((status = mysystem("nosuchcommand")) < 0){
		perror("mysystem");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if((status = system("who; exit 44")) <0){
		perror("mysystem");
		exit(1);
	}
	printf("exit status = %d\n", status);

	return 0;
}
