#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024

int main(int argc, char *argv[]){
	FILE *src, *dst;
	char buf[MAX_BUF];
	int count;
	
	if(argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}	
	if((src = fopen(argv[1], "rb")) == NULL){
		perror("fopen");
		exit(1);
	}
	if((dst = fopen(argv[2], "wb")) == NULL){
		perror("fopen");
		exit(1);
	}
	//binary file에서 read, write는 object단위로 동작
	//예제에서는 object가 크기를 1로 설정하고, MAX_BUF만큼 반복해서 buf에 저장
	while((count = fread(buf, 1, MAX_BUF, src)) > 0){ //end of file이면 반복 종료
		fwrite(buf, 1, count, dst); // 크기가 1인 object를 count만큼 반복하면서 buf에서 꺼내 dst에 입력
	}
	fclose(src); fclose(dst);

	return 0;
}
