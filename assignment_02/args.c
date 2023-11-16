//command에서 파리미터로 넘어온 file 이름을 출력하는 프로그램
#include <stdio.h>

int main(int argc, char *argv[]){
	int i;
	for(i = 0; i < argc; i++){ //파라미터의 수가 argc에 저장되기에 파라미터의 수 만큼 반복
		printf("argv[%d] = %s\n", i, argv[i]); //순서대로 argv에 저장된 file 이름을 출력
	}

	return 0;
}
