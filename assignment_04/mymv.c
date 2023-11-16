#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}
	//argv[1]의 file name을 argv[2]로 변경, 실패 시 error
	if(rename(argv[1], argv[2]) < 0){
		perror("rename");
		exit(1);
	}

	//기존 mv는 rename기능과 file move 기능이 모두 지원되지만, mymv의 경우 rename 기능만 지원	

	return 0;
}
