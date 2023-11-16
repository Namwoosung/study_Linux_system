#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *src, *dst;
	int ch;

	if(argc != 3){ //source�� destination�� �Ѿ���� ������ error
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if((src = fopen(argv[1], "rt")) == NULL){ //source�� read������ open, ���� �� error
		perror("fopen");
		exit(1);
	}
	if((dst = fopen(argv[2], "wt")) == NULL){ //destination�� write������ open
		perror("fopen");
		exit(1);
	}
	
	while((ch = fgetc(src)) != EOF){ //ch�� EOF�� return�� �� ����, fgetc�� src�� ������ character������ ����
		fputc(ch, dst); //ch�� dst�� �Է�
	}

	fclose(src); fclose(dst);

	return 0;
}
