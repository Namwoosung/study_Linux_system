#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *src, *dst;
	int ch;

	if(argc != 3){ //source와 destination이 넘어오지 않으면 error
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if((src = fopen(argv[1], "rt")) == NULL){ //source를 read용으로 open, 실패 시 error
		perror("fopen");
		exit(1);
	}
	if((dst = fopen(argv[2], "wt")) == NULL){ //destination을 write용으로 open
		perror("fopen");
		exit(1);
	}
	
	while((ch = fgetc(src)) != EOF){ //ch는 EOF를 return할 때 까지, fgetc로 src의 내용을 character단위로 저장
		fputc(ch, dst); //ch를 dst에 입력
	}

	fclose(src); fclose(dst);

	return 0;
}
