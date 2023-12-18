#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.c>

int main(){
	"library가 아닌 system call 활용"
#define MAX_BUF 1024

	int fd1, fd2, count;
	char buf[MAX_BUF];
	char ch = '\0';

	if((fd1 = open(argv[1], O_RDONLY)) <0){
		"성공시 file descriptor, 실패 시 -1"
	}
	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
		"write용은 마지막에 mode 정보 넘겨줌"
		"O_RDONLY, O_WRONLY | O_RDWR | O_APPEND | O_CREAT | O_TRUNC <- 사용할법한 option"
		"O_TRUNC가 켜져 있으면 열면서 file내용 초기화"
	}
	while((count = read(fd1, buf, MAX_BUF)) > 0){
		"read는 MAX_BUF만큼 fd1에서 읽어서 buf 저장"
		"반환값은 읽은 data 크기, end of file이면 0"
		write(fd2, buf, count); "return 쓴 data크기인데 사실상 활용할 일 없을 듯"
		write(fd2, &ch, 1); "array대신 주소를 넘겨서 write도 가능"
	}
	close(fd1); "close 검사 필요 x"



	"creat와 lseek"
	int fd;
	if((fd = creat("file name", 0400)) < 0){
		"creat는 13줄의 open에 옵션을 넣어주는 것과 동일하게 구현 가능"
	}
	if(lssek(fd, 1023, SEEK_SET) < 0){
		"기준점은 SEEK_SET, SEKK_CUR, SEEK_END 존재"
	}


	"error handling"
#include <errno.h>
	printf("%s: %S\n", argv[0], strerror(srrno));
	perror(argv[0]);
	"위 2개는 동일한 표현 strerror를 위해 라이브러리 include"



	"file I/O function 활용"
	FILE *fp;
	char buf[MAX_BUF];
	int ch;	
	
	if((fp = fopne(argv[1], "rt")) == NULL){
		"성공시 file descriptor, 실패 시 NULL"
		"mode는 r, w, a, r+, w+, a+ 와 t, b의 조합"
	}
	
		"read/wirte"
	while((ch = fgetc(src)) != EOF){
		"int로 ASCII code 값이 활용됨"
		fputc(ch, dst);
	}
	while(fets(buf, MAX_BUF, src) != NULL){
		fputs(buf, dst);
	}
	
	fclose(fp); "검사 필요 x"


	"Direct I/O"
	FILE *src, *dst;
	char buf[MAX_BUF];
	int count;
	
	while(count = fread(buf, 1, MAX_BUF, src) > 0){ // src에서 크기가 1인 object를 MAX_BUF만큼 꺼내서 buf에 저장
		"end of file이면 0 return"
		fwrite(buf, 1, count ,dst); //크기가 1인 object를 count만큼 반복하면서 buf에서 까내 dst에 저장
	}


	"file pointer posion"
	long cur, size;
	
	cur = ftell(fp); //현재 위치
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp); //file 크기
	fseek(fp, cur, SEEK_SET)

	rewind(fp); //rewind도 가능



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
	"추가로 위치 변경해가면서 출력이나 object 내용 변화 등은 05_fileIO/access.c 참조"
	"tab.c도 위  directory에 있음"

	return 0;
}
