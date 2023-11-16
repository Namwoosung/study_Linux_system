#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int Var;

int main(){
	Var = 1;
	//초기 프로세스의 pid를 저장
	pid_t pid = getpid();
	
	//초기 process이후 반복문이 돌 때마다 현재 존재하는 process만큼의 process가 생성됨
	//각 반복문 마다 Var *2를 해주면 현재 process 개수 계산 가능
	for(int i = 0; i < 4; i++){
		fork();
		Var *= 2;
	}

	//초기 process가 가지는 Var 값이 전체 생성된 process 개수
	if(pid == getpid()){
		printf("%d\n", Var);
	}

	return 0;
}
