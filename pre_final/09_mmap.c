#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//mycp3.c <- file 2개를 메모리에 올려서 copy
int main(int argc, char *argv[]){
	int fdin, fdout;
	char *src, *dst;
	struct stat statbuf;

	if(argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//file을 메모리에 올리려면 일단 open을 해줘야 함
	if((fdin = open(argv[1], O_RDONLY)) < 0){
		perror("open");
		exit(1);
	}
	if((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0){
		perror("open");
		exit(1);
	}

	//fdout은 현재 file크기가 0이기에 이대로 mmap하면 메모리에 할당하는 공간이 없음
	// -> lseek를 통해 빈 파일을 만들어 파일크기를 가지도록 해줘야 함
	if(fstat(fdin, &statbuf) < 0){
		perror("fstat");
		exit(1);
	}
	if(lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0){
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1);

	//실제 mmap
	if((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	if((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}

	//내용 복사 <- file을 메모리에 올렸으므로 pointer에 쓰는 것으로 복사가 가능
	//이렇게 안하면 file IO로 read, write system call을 활용해야 함
	memcpy(dst, src, statbuf.st_size);

	//munmap은 어짜피 프로세스가 종료되면서 자신이 가진 메모리가 반환되면서 사라지굇藪� 필수 과정은 아님

	close(fdin); close(fdout);
	return 0;
}


//dev/zeor file을 활용해 mmap하는 예제는 assignment_07/mipc.c 참고
