#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "record.h"

int main(int argc, char *argv[]){
	FILE *src, *dst;
	Record rec;

	if(argc != 3){
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
	}
	if((src = fopen(argv[1], "rt")) == NULL){
		perror("fopen");
		exit(1);
	}
	if((dst = fopen(argv[2], "wb")) == NULL){
		perror("fopen");
		exit(1);
	}
	while(fgets(rec.stud, SMAX, src)){
		*strchr(rec.stud, '\n') = '\0'; //strchr function은 문자열에서 두번째 파라미터의 문자를 검색하고 해당 위치를 반환
										//문자열 끝 부분의 \n을 찾아 NULL로 변환하는 작업
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0';
		fgets(rec.dept, DMAX, src);
		*strchr(rec.dept, '\n') = '\0';

		fwrite(&rec, sizeof(rec), 1, dst); //fgets로 받은 rec struct를 바이너리 방식으로 dst에 write
	}
	
	fclose(src);
	fclose(dst);

	return 0;
}
