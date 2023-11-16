#include <stdio.h>
#include <stdlib.h> //for exit function
#include <string.h> //for str function

enum { FALSE, TRUE };

void conv(char *fname){
	FILE *src, *dst;
	char fdest[40];
	int ch, first;
	//src open
	if((src = fopen(fname, "rt")) == NULL){
		perror("fopen");
		return ;
	}
	//src.t의 이름을 가지는 dst file open
	strcpy(fdest, fname);
	strcat(fdest, ".t");
	if((dst = fopen(fdest, "wt")) == NULL){
		perror("fopen");
		return ;
	}
	//first 변수는 현재 pointer가 line의 처음 위치인지 판단하는 변수
	first = TRUE;
	while((ch = getc(src)) != EOF){
		if(first && ch == '\t'){ //처음위치이면서 tab인 경우 빈 공간 4개 생성
			fputc(' ',dst);
			fputc(' ',dst);
			fputc(' ',dst);
			fputc(' ',dst);
		}
		else{ //처음 위치가 아니거나 tab이 아닌 경우
			fputc(ch, dst); //문자 입력
			if(first) //이후 first가 TREU로 설정되어 있으면
				first = FALSE; //first를 FLASE로 설정
			if(ch == '\n') //line 마지막에 도달하면
				first = TRUE; //first를 다시 TRUE로 설정
		}
	}
	
	fclose(src);
	fclose(dst);
}

int main(int argc, char *argv[]){
	while(--argc){ // argc - 1부터 1까지 반복
		conv(argv[argc]); //각 argv마다 conv 시행
	}

	return 0;
}
