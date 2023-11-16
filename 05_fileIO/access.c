#include <stdio.h> 
#include <unistd.h> //for file pointer
#include <stdlib.h> //for exit function
#include <string.h> //for strcpy function
#include "record.h"

void print_record(Record *rp){ //record�� �ν��Ͻ��� ����ϴ� �Լ�, puts function���� string ���
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

int main(int argc, char *argv[]){
	FILE *fp;
	Record rec;

	if(argc != 2){
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}
	//file open, read + write �뵵�� binary file open
	if((fp = fopen(argv[1], "r+b")) == NULL){
		perror("fopen");
		exit(1);
	}

	printf("-----Oredered Record List-----\n");
	//fread function���� record structure������ read�ؼ� ���
	while(fread(&rec, sizeof(rec), 1, fp)){
		print_record(&rec);
	}
	rewind(fp); //pointer ����ġ
	getchar();

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");
	//file pointer�� ó����ġ���� rec�� ũ���� �����ŭ �̵����� ���鼭 ������ �ٲ� ���
	fseek(fp, sizeof(rec) * 2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec) * 5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	fseek(fp, sizeof(rec) * 1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	fseek(fp, sizeof(rec) * 3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	fseek(fp, sizeof(rec) * 0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	fseek(fp, sizeof(rec) * 4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	getchar();

	printf("-----Updated Record List-----\n");

	fseek(fp, sizeof(rec) * 2L, SEEK_SET); //file pointer�� 3��° object�� �̵�
	fread(&rec, sizeof(rec), 1, fp); //rec�� ����
	strcpy(rec.dept, "Movie"); //rec�� ����
	fseek(fp, sizeof(rec) * 2L, SEEK_SET); //�ٽ� 3��° object�� �̵�
	fwrite(&rec, sizeof(rec), 1, fp); //������ rec���� file�� ����
	//6��° object�� ���������� ����
	fseek(fp, sizeof(rec) * 5L, SEEK_SET); 
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec) * 5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);	
	//pointer�� ó����ġ�� �����ϰ� ��� ������ ���
	rewind(fp);
	while(fread(&rec, sizeof(rec), 1, fp)){
		print_record(&rec);
	}

	fclose(fp);

	return 0;
}
