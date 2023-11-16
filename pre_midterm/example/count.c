#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}
	
	FILE *fp;
	int ch;
	int count = 0;

	if((fp = fopen(argv[1], "rt")) == NULL){
		perror("fopen");
		exit(1);
	}
	while((ch = fgetc(fp)) != EOF){
		if(ch == ' ')
			count++;
	}
	fclose(fp);
	
	printf("%d\n", count);

	return 0;
}
