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

	//command argument�� read�� file�� copy�� file�� �Է����� ����
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

	//read�� file�� metadata�� ������
	if(fstat(fdin, &statbuf) < 0){
		perror("fstat");
		exit(1);
	}

	//fdout�� ���� lseek�� write�� ���ִ� ����
	//���� fdout�� file�� �������� ����, �� file ũ�Ⱑ 0�̱⿡ memory�� mapping�� ������ ����
	//�츮�� fdin file�� �״�� ���� ������ ���̱⿡ �� ũ�⸸ŭ�� ũ��� fdout�� ����������
	//leek�� null pointer write�� ���� fdin file�� ������ ũ�⸦ �����鼭 �� file�� fdout�� ����
	if(lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0){
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1);

	//src�� dst�� ���� mapping�ϰ� �ش� ��ġ�� pointer�� ����
	//MAP_SHARED �ɼ��� �ٸ� process�� ������ �������� ���� �����ε�, ���⼭�� �ش� process�� ����ϱ⿡ ��ǻ� �ǹ̾��� �ɼ�
	if((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	if((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}

	//src���� file ũ�⸸ŭ�� data�� dst pointer��ġ�� ����
	memcpy(dst, src, statbuf.st_size);
	
	close(fdin); close(fdout);

	return 0;
}
