#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //for toupper function

int main(int argc, char *argv[]){
	FILE *fp;
	char ch;

	if(argc != 2){
		fprintf(stderr, "Usage : %s filename\n", argv[0]);
		exit(1);	
	}
	
	if((fp = fopen(argv[1], "rt")) == NULL){
		perror("fopen");
		exit(1);
	}

	while((ch = fgetc(fp)) != EOF){ //EOF까지 character를 하나씩 받아
		putchar(toupper(ch)); //대문자로 변환 후 출력
	}

	fclose(fp);

	return 0;
}
