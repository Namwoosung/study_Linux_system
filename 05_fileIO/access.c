#include <stdio.h> 
#include <unistd.h> //for file pointer
#include <stdlib.h> //for exit function
#include <string.h> //for strcpy function
#include "record.h"

void print_record(Record *rp){ //record의 인스턴스를 출력하는 함수, puts function으로 string 출력
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
	//file open, read + write 용도로 binary file open
	if((fp = fopen(argv[1], "r+b")) == NULL){
		perror("fopen");
		exit(1);
	}

	printf("-----Oredered Record List-----\n");
	//fread function으로 record structure단위로 read해서 출력
	while(fread(&rec, sizeof(rec), 1, fp)){
		print_record(&rec);
	}
	rewind(fp); //pointer 원위치
	getchar();

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");
	//file pointer를 처음위치에서 rec의 크기의 배수만큼 이동시켜 가면서 순서를 바꿔 출력
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

	fseek(fp, sizeof(rec) * 2L, SEEK_SET); //file pointer를 3번째 object로 이동
	fread(&rec, sizeof(rec), 1, fp); //rec에 저장
	strcpy(rec.dept, "Movie"); //rec값 변경
	fseek(fp, sizeof(rec) * 2L, SEEK_SET); //다시 3번째 object로 이동
	fwrite(&rec, sizeof(rec), 1, fp); //변경한 rec값을 file에 저장
	//6번째 object도 마찬가지로 변경
	fseek(fp, sizeof(rec) * 5L, SEEK_SET); 
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec) * 5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);	
	//pointer를 처음위치로 변경하고 모든 내용을 출력
	rewind(fp);
	while(fread(&rec, sizeof(rec), 1, fp)){
		print_record(&rec);
	}

	fclose(fp);

	return 0;
}
