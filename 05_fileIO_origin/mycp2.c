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
	//binary file���� read, write�� object������ ����
	//���������� object�� ũ�⸦ 1�� �����ϰ�, MAX_BUF��ŭ �ݺ��ؼ� buf�� ����
	while((count = fread(buf, 1, MAX_BUF, src)) > 0){ //end of file�̸� �ݺ� ����
		fwrite(buf, 1, count, dst); // ũ�Ⱑ 1�� object�� count��ŭ �ݺ��ϸ鼭 buf���� ���� dst�� �Է�
	}
	fclose(src); fclose(dst);

	return 0;
}
