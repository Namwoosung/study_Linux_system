#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	int fdin, fdout;
	char *src, *dst;
	struct stat statbuf;

	//command argument로 read할 file과 copy할 file을 입력으로 받음
	if(argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	
	if((fdin = open(argv[1], O_RDONLY)) < 0){
		perror("open");
		exit(1);
	}
	if((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0){
		perror("open");
		exit(1);
	}

	//read할 file의 metadata를 가져옴
	if(fstat(fdin, &statbuf) < 0){
		perror("fstat");
		exit(1);
	}

	//fdout에 대해 lseek와 write를 해주는 이유
	//현재 fdout은 file이 존재하지 않음, 즉 file 크기가 0이기에 memory에 mapping할 공간이 없음
	//우리는 fdin file을 그대로 전부 복사할 것이기에 그 크기만큼의 크기로 fdout을 만들어줘야함
	//leek와 null pointer write를 통해 fdin file과 동일한 크기를 가지면서 빈 file로 fdout을 설정
	if(lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0){
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1);

	//src와 dst를 각각 mapping하고 해당 위치를 pointer로 받음
	//MAP_SHARED 옵션은 다른 process와 공유할 것인지에 대한 여부인데, 여기서는 해당 process만 사용하기에 사실상 의미없는 옵션
	if((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	if((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}

	//src부터 file 크기만큼의 data를 dst pointer위치에 복사
	memcpy(dst, src, statbuf.st_size);
	
	close(fdin); close(fdout);

	return 0;
}
