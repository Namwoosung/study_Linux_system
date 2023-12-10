#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//mycp3.c <- file 2���� �޸𸮿� �÷��� copy
int main(int argc, char *argv[]){
	int fdin, fdout;
	char *src, *dst;
	struct stat statbuf;

	if(argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//file�� �޸𸮿� �ø����� �ϴ� open�� ����� ��
	if((fdin = open(argv[1], O_RDONLY)) < 0){
		perror("open");
		exit(1);
	}
	if((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0){
		perror("open");
		exit(1);
	}

	//fdout�� ���� fileũ�Ⱑ 0�̱⿡ �̴�� mmap�ϸ� �޸𸮿� �Ҵ��ϴ� ������ ����
	// -> lseek�� ���� �� ������ ����� ����ũ�⸦ �������� ����� ��
	if(fstat(fdin, &statbuf) < 0){
		perror("fstat");
		exit(1);
	}
	if(lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0){
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1);

	//���� mmap
	if((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	if((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}

	//���� ���� <- file�� �޸𸮿� �÷����Ƿ� pointer�� ���� ������ ���簡 ����
	//�̷��� ���ϸ� file IO�� read, write system call�� Ȱ���ؾ� ��
	memcpy(dst, src, statbuf.st_size);

	//munmap�� ��¥�� ���μ����� ����Ǹ鼭 �ڽ��� ���� �޸𸮰� ��ȯ�Ǹ鼭 �������⿡ �ʼ� ������ �ƴ�

	close(fdin); close(fdout);
	return 0;
}


//dev/zeor file�� Ȱ���� mmap�ϴ� ������ assignment_07/mipc.c ����
