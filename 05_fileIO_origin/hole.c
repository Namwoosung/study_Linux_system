#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int fd;
	char ch = '\0';	

	if((fd = creat("file.hole", 0400)) < 0){ //file.hole 파일 생성, mode는 0400, fd에는 3이 저장될 것
		perror("creat"); //함수 return 값이 -1이라면 오류이므로 error메시지로 creat 전송
		exit(1); //강제종료
	}
	
	if(lseek(fd ,1023, SEEK_SET) < 0){ //lseek를 통해 file의 포인터를 처음위치 기준에서 1023 byte만큼 이동
		perror("lseek"); //실패시 error 전송
		exit(1);
	}
	write(fd, &ch, 1); //1024위치에 null을 write, 결과적으로 1024byte공간을 차지하면서 모든 내용은 비어있는 file 생성

	close(fd);

	return 0;
}
