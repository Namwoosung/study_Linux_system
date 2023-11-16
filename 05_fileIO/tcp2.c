#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 256

int main(int argc, char *argv[]){
	FILE *src, *dst;
	char buf[MAX_BUF];
	
	if(argc != 3){ //source�� destination�� ����� ���� ������ error
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if((src = fopen(argv[1], "rt")) == NULL){ //src�� read������ open
		perror("fopen");
		exit(1);
	}
	if((dst = fopen(argv[2], "wt")) == NULL){ //dst�� write������ open
		perror("fopen");
		exit(1);
	}
	
	while(fgets(buf, MAX_BUF, src) != NULL){ //fgets�� buf�� �� �پ� ����, NULL return�� file�� end�̹Ƿ� �ݺ�����
		fputs(buf, dst); //fputs�� buf�� ������ dst�� �Է�
	}

	fclose(src); fclose(dst);

	return 0;
}
