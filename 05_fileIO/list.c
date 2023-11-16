//command line에서 파리미터로 온 file 내용을 line number와 함께 출력하는 프로그램
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024 //read할 MAX_BUF 설정

int main(int argc, char *argv[]){
	FILE *fp;
	char buf[MAX_BUF];
	int line;

	if(argc != 2){ //파라미터로 file이 넘어오지 않으면 error처리
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	if((fp = fopen(argv[1], "rt")) == NULL){ //text file을 read용으로 open, 실패시 error처리
		perror("fopen");
		exit(1);
	}

	line = 1;
	while(fgets(buf, MAX_BUF, fp)){ //file 내용을 line마다 읽어 buf에 저장
		printf("%4d: %s", line++, buf); //line number와 file 내용 출력
	}//이후 file 끝 도달시 fgets함수가 NULL을 return하며 반복 종료

	fclose(fp);

	return 0;
}
