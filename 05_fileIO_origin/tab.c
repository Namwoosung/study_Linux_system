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
	//src.t�� �̸��� ������ dst file open
	strcpy(fdest, fname);
	strcat(fdest, ".t");
	if((dst = fopen(fdest, "wt")) == NULL){
		perror("fopen");
		return ;
	}
	//first ������ ���� pointer�� line�� ó�� ��ġ���� �Ǵ��ϴ� ����
	first = TRUE;
	while((ch = getc(src)) != EOF){
		if(first && ch == '\t'){ //ó����ġ�̸鼭 tab�� ��� �� ���� 4�� ����
			fputc(' ',dst);
			fputc(' ',dst);
			fputc(' ',dst);
			fputc(' ',dst);
		}
		else{ //ó�� ��ġ�� �ƴϰų� tab�� �ƴ� ���
			fputc(ch, dst); //���� �Է�
			if(first) //���� first�� TREU�� �����Ǿ� ������
				first = FALSE; //first�� FLASE�� ����
			if(ch == '\n') //line �������� �����ϸ�
				first = TRUE; //first�� �ٽ� TRUE�� ����
		}
	}
	
	fclose(src);
	fclose(dst);
}

int main(int argc, char *argv[]){
	while(--argc){ // argc - 1���� 1���� �ݺ�
		conv(argv[argc]); //�� argv���� conv ����
	}

	return 0;
}
