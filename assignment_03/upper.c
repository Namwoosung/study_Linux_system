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

	while((ch = fgetc(fp)) != EOF){ //EOF���� character�� �ϳ��� �޾�
		putchar(toupper(ch)); //�빮�ڷ� ��ȯ �� ���
	}

	fclose(fp);

	return 0;
}
