#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //file pointer ���� function�� ���� ���̺귯��

#define MAX_BUF 1024

long filesize(FILE *fp){
	long cur, size;
	
	cur = ftell(fp); //���� pointer ��ġ ����
	fseek(fp, 0L, SEEK_END); //size Ȯ���� ���� ������ ��ġ�� poitner �̵�
	size = ftell(fp); //size ����
	fseek(fp, cur, SEEK_SET); //�Լ� ȣ�� ���� pointer��ġ�� ����
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
	//source file�� size�� ������ ����
	size = filesize(src) / 2;

	//MAX_BUF��ŭ file�� �����ϴµ�, size�� ũ�Ⱑ �Ѿ�� �������� ����
	//MAX_BUF��ŭ write�ϴٰ� size < MAX_BUF�� �Ǵ� ���� size��ŭ�� write�ϰ� �ݺ� ����
	while(size > 0){
		count = (size > MAX_BUF) ? MAX_BUF : size;
		fread(buf, 1, count, src);
		fwrite(buf, 1, count, dst1);
		size -= count;
	}
	//source�� ���� �κ��� dst2�� ����
	while((count = fread(buf, 1, MAX_BUF, src)) > 0){
		fwrite(buf, 1, count, dst2);
	}
	
	fclose(src);
	fclose(dst1);
	fclose(dst2);

	return 0;
}
