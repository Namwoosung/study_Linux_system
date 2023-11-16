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
		*strchr(rec.stud, '\n') = '\0'; //strchr function�� ���ڿ����� �ι�° �Ķ������ ���ڸ� �˻��ϰ� �ش� ��ġ�� ��ȯ
										//���ڿ� �� �κ��� \n�� ã�� NULL�� ��ȯ�ϴ� �۾�
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0';
		fgets(rec.dept, DMAX, src);
		*strchr(rec.dept, '\n') = '\0';

		fwrite(&rec, sizeof(rec), 1, dst); //fgets�� ���� rec struct�� ���̳ʸ� ������� dst�� write
	}
	
	fclose(src);
	fclose(dst);

	return 0;
}
