#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int fd[2];
	pid_t child;

	if(pipe(fd) < 0 ){
		perror("pipe");
		exit(1);
	}
	child = fork();
	if(child == (pid_t)(-1)){
		perror("fork");
		exit(1);
	} else if(child == (pid_t)(0)){
		//STDOUT과 read용 pipe close
		close(1);
		close(fd[0]);
		//write용 pipe인 fd[1]과 close된 file descriptor 중 가장 낮은 숫자를 duplicate
		//결과적으로 STROU과 duplicate
		//따라서 해당 process에서 STDOUT으로 출력되던 것이 pipe로 write될 것
		if(dup(fd[1]) == -1){
			perror("dup");
			exit(1);
		}
		//ls -l실행, PATH에서 명령어를 찾아서 수행
		//원래는 ls -l의 결과가 STDOUT으로 출력되지만, 현재는 fd[1]으로 출력
		if(execlp("ls", "ls", "-l", NULL) == -1){
			perror("execlp");
			exit(1);
		}

	} else{
		//STDIN과 write용 pipe close
		close(0);
		close(fd[1]);
		//fd[0]과 STDIN duplicate
		if(dup2(fd[0], 0) == -1){
			perror("dup");
			exit(1);
		}
		//more을 실행
		//기존에는 STDIN으로 들어온 값을 more해서 출력
		//현재는 pipe로 read해서 출력할 것
		if(execlp("more", "more", NULL) == -1){
			perror("execlp");
			exit(1);
		}
	}

	return 0;
}
