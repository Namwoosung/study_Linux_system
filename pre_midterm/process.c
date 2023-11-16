#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

	"process 관련 compile시에는 LDFALGS = -lpthread를 설정해줘야 함"

int main(){
	//둘 다 가능
	int pid;
	pid_t pid2;
	
	"fork 사용법"
	if((pid = fork()) < 0){
		"fork는 성공 시 child의 pid, 혹은 0, 실패시 -1 반환"
		perror("fork");
		exit(1);
	}else if(pid == 0){
		"child process"
	}else{
		"parent process"
	}
	
	
	"exit 사용법"
	if(atexit(myexit1) != 0){
		"exit관련 함수는 성공 시 0, 실패 시 0이 아닌 값 반환"
		perror("atexit");
		exit(1); "파라미터는 status, parent process가 에게 exit 상태를 알려주는 역할"
	}


	"wait 사용법"
	int status;
	pid_t pid;
	if((pid = fork()) < 0){}
	else if(pid == 0) {}
	else {
		wait(&status);
		"wait에 status의 주소를 넘겨주면 child의 exit 상태를 확인 가능, 필요 없으면 NULL"
		"wait의 반환 값은 process ID인데 사실 사용 x"
		"사실 status랑 반환값 둘 다 특별한 상황아니면 안써서 wait(NULL)이 주로 사용"
		if(waitpid(pid, &status, 0) < 0){
			"waitpid도 pid설정 빼고는 똑같음, 마지막 파리미터는 옵션인데 그냥 0"
			perror("waitpid");
			exit(1)
		}
	}
	
	
	"exec 사용법"
	pid_t pid;
	char *argv[] = {"mycp, exec.c, exec2.c, NULL"};
	if((pid = fork()) < 0) {}
	else if(pid == 0){
		"exec 관련 함수들은 성공시 반환 없음, 실패 시 -1 반환"
		"exec 첫번째 파라미터는 정확한 path, 이후 뒤에는 argv0 부터 null"
		if(execl("/home/lsp15/05_filrIO/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0){
			perror("execl");
			exit(1);
		}
		if(execv("/home/lsp15/05_fileIO/mycp", argv) < 0){
			perror("execv");
			exit(1);
		}
		if(execlp("env", "env", NULL) < 0){
			"execlp는 PATH에서 파일을 찾으니 경로 명시 없이 filename, argv0부터 NULL 입력"
			perror("execlp");
			exit(1)
		}
		"추가로 execle, execve는 세번째 인자로 환경변수의 array까지 넘겨줌"
	}


	"환경변수 사용법"
	int main(int argc, char *argv[], char *envp[]) "파라미터로 가져옴"
	extern char **environ; "변수로 가져오기" 
	for(i = 0; envp[i] != NULL; i++){
		printf("%s\n",envp[i]);
	}
	char **p;
	for(p = envp; *p != NULL; p++){
		printf("%s\n", *p);
	}


	"system 사용법"
	int status;
	if((status = system("date; who")) < 0){
		"system은 성공 시 종료 status, 실패 시 -1 반환"
		perror("system");
		exit(1);
	}


	"thread 사용법"
	pthread_t tid1;
		"thread 함수들은 성공 시 0반환, 실패 시 음수 반환"
	if(pthread_create(&tid1, NULL, (void *)function, (void *)arg) < 0){
		"처음 파라미터에서 tid를 받아오고, 2번째는 attribute인데 그냥 NULL넣으셈"
		"function이랑 파라미터, 파라미터 줄 거 많으면 struct로 전달"
		perror("pthread_create");
		exit(1);
	}

	"pthread_exit(NULL)은 function 내부 마지막에 선언되어 있을 것"
	
	if(pthread_join(tid1, NULL) < 0){
		"tid1이 종료될 때 까지 wait"
		perror("pthread_join");
		exit(1);
	}	

	"struct활용과 세부적인 thread 예시는 assignment05/tadd.c 참고"

	return 0;
}
