#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "usage fault");
		exit(1);
	}

	char *ptr = (char *)malloc(sizeof(char) * sizeof(argv[1]));
	int num = atoi(argv[2]);
	int i;

	strcat(ptr, argv[1]);	

	for(i = 0; i < num; i++){
		putchar(*ptr);
		ptr++;
	}
	printf("\n");
	
	return 0;
}
