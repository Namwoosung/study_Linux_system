#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.c>

int main(){
	"library�� �ƴ� system call Ȱ��"
#define MAX_BUF 1024

	int fd1, fd2, count;
	char buf[MAX_BUF];
	char ch = '\0';

	if((fd1 = open(argv[1], O_RDONLY)) <0){
		"������ file descriptor, ���� �� -1"
	}
	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
		"write���� �������� mode ���� �Ѱ���"
		"O_RDONLY, O_WRONLY | O_RDWR | O_APPEND | O_CREAT | O_TRUNC <- ����ҹ��� option"
		"O_TRUNC�� ���� ������ ���鼭 file���� �ʱ�ȭ"
	}
	while((count = read(fd1, buf, MAX_BUF)) > 0){
		"read�� MAX_BUF��ŭ fd1���� �о buf ����"
		"��ȯ���� ���� data ũ��, end of file�̸� 0"
		write(fd2, buf, count); "return �� dataũ���ε� ��ǻ� Ȱ���� �� ���� ��"
		write(fd2, &ch, 1); "array��� �ּҸ� �Ѱܼ� write�� ����"
	}
	close(fd1); "close �˻� �ʿ� x"



	"creat�� lseek"
	int fd;
	if((fd = creat("file name", 0400)) < 0){
		"creat�� 13���� open�� �ɼ��� �־��ִ� �Ͱ� �����ϰ� ���� ����"
	}
	if(lssek(fd, 1023, SEEK_SET) < 0){
		"�������� SEEK_SET, SEKK_CUR, SEEK_END ����"
	}


	"error handling"
#include <errno.h>
	printf("%s: %S\n", argv[0], strerror(srrno));
	perror(argv[0]);
	"�� 2���� ������ ǥ�� strerror�� ���� ���̺귯�� include"



	"file I/O function Ȱ��"
	FILE *fp;
	char buf[MAX_BUF];
	int ch;	
	
	if((fp = fopne(argv[1], "rt")) == NULL){
		"������ file descriptor, ���� �� NULL"
		"mode�� r, w, a, r+, w+, a+ �� t, b�� ����"
	}
	
		"read/wirte"
	while((ch = fgetc(src)) != EOF){
		"int�� ASCII code ���� Ȱ���"
		fputc(ch, dst);
	}
	while(fets(buf, MAX_BUF, src) != NULL){
		fputs(buf, dst);
	}
	
	fclose(fp); "�˻� �ʿ� x"


	"Direct I/O"
	FILE *src, *dst;
	char buf[MAX_BUF];
	int count;
	
	while(count = fread(buf, 1, MAX_BUF, src) > 0){ // src���� ũ�Ⱑ 1�� object�� MAX_BUF��ŭ ������ buf�� ����
		"end of file�̸� 0 return"
		fwrite(buf, 1, count ,dst); //ũ�Ⱑ 1�� object�� count��ŭ �ݺ��ϸ鼭 buf���� � dst�� ����
	}


	"file pointer posion"
	long cur, size;
	
	cur = ftell(fp); //���� ��ġ
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp); //file ũ��
	fseek(fp, cur, SEEK_SET)

	rewind(fp); //rewind�� ����



	"conver text to binary"
	typedef struct{
		char stud[SMAX];
		char num[NMAX];
		char dept[DMAX];
	} Record;

	Record rec;
	
	while(fgets(rec.stud, SMAX, src)){
		*strchr(rec.stud, '\n') = '\0';
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0';
		
		fwrite(rec, sizeof(rec), 1, dst);
	}

	while(fread(&rec, sizeof(rec), 1, fp)){

	}
	"�߰��� ��ġ �����ذ��鼭 ����̳� object ���� ��ȭ ���� 05_fileIO/access.c ����"
	"tab.c�� ��  directory�� ����"

	return 0;
}
