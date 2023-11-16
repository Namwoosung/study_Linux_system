#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 256

int main(int argc, char *argv[]){
	FILE *src, *dst;
	char buf[MAX_BUF];
	
	if(argc != 3){ //source와 destination이 제대로 오지 않으면 error
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if((src = fopen(argv[1], "rt")) == NULL){ //src를 read용으로 open
		perror("fopen");
		exit(1);
	}
	if((dst = fopen(argv[2], "wt")) == NULL){ //dst를 write용으로 open
		perror("fopen");
		exit(1);
	}
	
	while(fgets(buf, MAX_BUF, src) != NULL){ //fgets로 buf에 한 줄씩 저장, NULL return시 file의 end이므로 반복종료
		fputs(buf, dst); //fputs로 buf의 내용을 dst에 입력
	}

	fclose(src); fclose(dst);

	return 0;
}
