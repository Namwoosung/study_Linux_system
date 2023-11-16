#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_BUF 256

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	char buf[MAX_BUF];

	if(getcwd(buf, MAX_BUF) == NULL){
		perror("getcwd");
		exit(1);
	}
	printf("%s\n", buf);
	
	if(chdir(argv[1]) < 0) {
		perror("chdir");
		exit(1);
	}

	if(getcwd(buf, MAX_BUF) == NULL){
		perror("getcwd");
		exit(1);
	}
	printf("%s\n", buf);
	return 0;
}
