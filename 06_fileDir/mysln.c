#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	if(symlink(argv[1], argv[2]) < 0){ //argv[1]의 symbolic link로 atgv[2] 생성
		perror("symlink");
		exit(1);
	}
	return 0;
}
