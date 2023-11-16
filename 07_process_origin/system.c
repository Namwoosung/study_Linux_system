#include <stdio.h>
#include <stdlib.h>

int main(){
	int status;
	//system function은 string으로 command 입력을 받아 시행해주는 함수
	//내부적으로 fork, exec를 수행하고, parent는 waitpid로 기다리도록 구현
	//status는 termination status
	if((status = system("date")) < 0){
		perror("sytem");
		exit(1);
	}
	printf("exit status = %d\n", status);
	//해당 command는 존재하지 않으니 error가 발생할 것
	if((status = system("nosuchcommand")) < 0 ){
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
	//여러개의 command를 ;로 구분해서 전달 가능
	if((status = system("who; exit 44")) <0){
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
	
	return 0;
}
