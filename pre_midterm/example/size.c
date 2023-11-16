#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage fail");
		exit(1);
	}

	struct stat statbuf;
	DIR *dp;
	struct dirent *dep;

	if((dp = opendir(".")) == NULL){
		perror("opendir");
		exit(1);
	}
	while(dep = readdir(dp)){
		
	}


	return 0;
}
