#include <stdio.h>
#include <stdlib.h>
//exit handler로 등록할 함수 2개 선언
void myexit1(){
	printf("first exit handler\n");
}

void myexit2(){
	printf("second exit handler\n");
}

int main(){
	//myexit2를 exit handler로 설정, 성공 시 0, 실패 시 다른 수 반환이므로 0인지 확인
	if(atexit(myexit2) != 0){
		perror("atexit");
		exit(1);
	}

	if(atexit(myexit1) != 0){
		perror("atexit");
		exit(1);
	}

	if(atexit(myexit1) != 0){
		perror("atexit");
		exit(1);
	}
	
	printf("main is done\n");
	//main에서 return되면서 C start-up routine이 동작 이후 exit function이 동작
	//exit function에서 exit handler로 등록한 함수들을 차례로 수행
	//동일한 function도 여러번  등록가능
	return 0;
}
