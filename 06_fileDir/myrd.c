#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}
	//argv[1]¿« name¿« directory∏¶ remove
	if(rmdir(argv[1]) < 0){
		perror("rmdir");
		exit(1);
	}
	return 0;
}
