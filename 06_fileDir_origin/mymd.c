#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}
	//argv[1]의 name을 가지는 directory를 755 mode를 가지게 make
	if(mkdir(argv[1], 0755) < 0){
		perror("mkdir");
		exit(1);
	}

	return 0;
}
