#include <stdio.h>
#include <unistd.h>

#define MAX_CMD 256
//cmd를 실제로 실행하는 것이 아니라 1초 쉬면서 실행하는 것처럼 보이도록 동작하는 함수
void DoCmd(char *cmd){
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main(){
	char cmd[MAX_CMD];
	
	//q입력 전까지 무한 반복
	while(1){
		printf("CMD> ");
		//console에서 cmd를 입력으로 받음
		fgets(cmd, MAX_CMD, stdin);
		if(cmd[0] == 'q')
			break;
		DoCmd(cmd);
	}
	//해당 프로그램은 단일 process로 수행되도록 구성되었기에 사용자가 cmd를 입력하면
	//해당 입력에 대한 처리가 끝난 뒤에 다시 CMD>를 출력하면서 대기할 것
	return 0;
}
