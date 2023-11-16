#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024

int main(int argc, char *argv[]){
	FILE *src1, *src2, *dst;
	char buf[MAX_BUF];
	int count;

	if(argc != 4){
		fprintf(stderr, "Usage: %s source1 source2 destination\n", argv[0]);
		exit(1);
	}
	//file open
	if((src1 = fopen(argv[1], "rb")) == NULL){
		perror("fopen");
		exit(1);
	}
	if((src2 = fopen(argv[2], "rb")) == NULL){
		perror("fopen");
		exit(1);
	}
	if((dst = fopen(argv[3], "wb")) == NULL){
		perror("fopen");
		exit(1);
	}
	//src1의 file을 먼저 dst에 전부 wrtie
	while((count = fread(buf, 1, MAX_BUF, src1)) >0){
		fwrite(buf, 1, count, dst);
	}
	//src2의 file을 dst에 전부 write
	while((count = fread(buf, 1, MAX_BUF, src2)) >0){
		fwrite(buf, 1, count, dst);
	}
	
	fclose(src1);
	fclose(src2);
	fclose(dst);

	return 0;
}
