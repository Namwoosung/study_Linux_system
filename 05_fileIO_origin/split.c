#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //file pointer 제어 function을 위한 라이브러리

#define MAX_BUF 1024

long filesize(FILE *fp){
	long cur, size;
	
	cur = ftell(fp); //현재 pointer 위치 저장
	fseek(fp, 0L, SEEK_END); //size 확인을 위해 마지막 위치로 poitner 이동
	size = ftell(fp); //size 저장
	fseek(fp, cur, SEEK_SET); //함수 호출 이전 pointer위치로 복귀
	return (size);
}

int main(int argc, char *argv[]){
	FILE *src, *dst1, *dst2;
	char buf[MAX_BUF];
	int size, count;
	
	if(argc != 4){
		fprintf(stderr, "Usage: %s source dst1 dst2\n", argv[0]);
		exit(1);
	}	
	//file open
	if((src = fopen(argv[1], "rb")) == NULL){
		perror("fopen");
		exit(1);
	}
	if((dst1 = fopen(argv[2], "wb")) == NULL){
		perror("fopen");
		exit(1);
	}
	if((dst2 = fopen(argv[3], "wb")) == NULL){
		perror("fopen");
		exit(1);
	}
	//source file의 size의 절반을 저장
	size = filesize(src) / 2;

	//MAX_BUF만큼 file을 복사하는데, size의 크기가 넘어가기 전까지만 복사
	//MAX_BUF만큼 write하다가 size < MAX_BUF가 되는 순간 size만큼만 write하고 반복 종료
	while(size > 0){
		count = (size > MAX_BUF) ? MAX_BUF : size;
		fread(buf, 1, count, src);
		fwrite(buf, 1, count, dst1);
		size -= count;
	}
	//source의 남은 부분을 dst2에 저장
	while((count = fread(buf, 1, MAX_BUF, src)) > 0){
		fwrite(buf, 1, count, dst2);
	}
	
	fclose(src);
	fclose(dst1);
	fclose(dst2);

	return 0;
}
