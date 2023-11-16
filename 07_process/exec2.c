#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
	pid_t pid;
	char *argv[] = {"mycp", "exec.c", "exec2.c", NULL};	//execv에서 파라미터로 넘겨줄 list 선언
	//execl
	if((pid = fork()) <0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){ //child process일 경우
		//child process는 execl을 수행하면서 mycp를 동작시키는 process로 변화
		//execl은 파라미터를 리스트형태로 넘겨줌
		//처음에 실행시킬 파일의 path 이후 argv에 넘겨줄 값, 마지막은 NULL로 명시
		if(execl("/home/lsp15/05_fileIO/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0 ){
			perror("execl");
			exit(1);
		}
	}
	//parent process의 경우 자식이 끝날때까지 기다림
	if(waitpid(pid, NULL, 0) < 0){
		perror("waitpid");
		exit(1);
	}
	
	//execv
	//다시 새로운 process 생성
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		//child process는 execv를 통해 mycp 실행, array형태로 파라미터 전달
		if(execv("/home/lsp15/05_fileIO/mycp", argv) < 0){
			perror("execv");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0){
		perror("waitpid");
		exit(1);
	}
	//execlp
	//execlp는 프로그램을 실행시킬 때 PATH에서 탐색해서 실행
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		//env 프로그램을 PATH에서 찾아서 실행시키라는 의미
		if(execlp("env", "env", NULL) < 0){
			perror("execlp");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0 ){
		perror("waitpid");
		exit(1);
	}

	return 0;
}
