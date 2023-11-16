//파일을 읽어 그대로 복사하는 프로그램, system call을 직접 활용
#include <stdio.h>
#include <fcntl.h> //system call for File I/O
#include <unistd.h> //system call for File I/O
#include <stdlib.h> //for exit() function

#define MAX_BUF 1024 //MAX buffer 설정

int main(int argc, char *argv[]){
	int fd1, fd2, count;
	char buf[MAX_BUF];

	if(argc !=3 ){ //argv[1]의 file을 argv[2] file로 복사하기 위해 argc == 3이어야 하기에 검사
		printf("Usage: %s source destination\n", argv[0]);
		exit(1); //error로 인한 강제 종료
	}

	if((fd1 = open(argv[1], O_RDONLY)) < 0){ //argv[1]의 file을 read용으로 open, descriptot를 fd1에 저장(3이 저장될 것)
		perror("open"); //open 실패 시 error 메시지 전송
		exit(1);
	}
	
	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){ //ragv[2]의 file을 write용으로 open, copy를 위해 기존 내용을 모두 지우고, file이 없다면 생성, mode는 644
		perror("open");
		exit(1);
	}	
	
	while((count = read(fd1, buf, MAX_BUF)) > 0){ // count 변수에 MAX_BUF혹은 fd1의 남은 크기만큼 저장
		write(fd2, buf, count); //count만큼 fd2에 write
	} //MAX_BUF만큼 반복해서 write하다가 fd1의 남은 file크기가 MAX_BUF보다 작으면 해당 남은 부분을 write
	//이후 end of file에 도달해 0이 되면 반복 종료

	//file close
	close(fd1); close(fd2);

	return 0;
}
