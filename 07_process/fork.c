#include <stdio.h>
#include <sys/types.h> //for typedef
#include <unistd.h> //for fork function

//전역변수로 Var 생성
int Var = 6;
char Buf[] = "a write to stdout\n";

int main(){
	//지역변수로 var 생성
	int var;
	pid_t pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf) - 1); //Buf 내용을 출력
	printf("Before fork\n");

	if((pid = fork()) == 0){ //child process인 경우 fork()의 반환값이 0
		/*child*/
		//child porcess에서만 변수의 값 +1
		Var++;
		var++;
	}
	else{ //parent process일 경우 fork()의 반환값이 child process의 PID
		/*parent*/
		sleep(2);
	}
	//parent process에서 fork을 진행하면 child process가 생성
	//생성 시점에서는 parent와 child의 변수의 값이 동일(child는 parent를 복사해서 생성)
	//이후에는 서로 다른 memory를 사용하기에 독립적인 process로 동작
	//chuld는 if 문에 들어가 변수 증가, 그러나 parent는 else문으로 들어가 변수 증가 x
	//이후 scheduling algorithm에 의해 child와 parent 중 적절하게 실행
	//parent는 sleep(2)이기에 parent process먼저 출력 후 이후 parent process가 출력
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);

	return 0;
}
