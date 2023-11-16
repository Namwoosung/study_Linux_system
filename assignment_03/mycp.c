//������ �о� �״�� �����ϴ� ���α׷�, system call�� ���� Ȱ��
#include <stdio.h>
#include <fcntl.h> //system call for File I/O
#include <unistd.h> //system call for File I/O
#include <stdlib.h> //for exit() function

#define MAX_BUF 1024 //MAX buffer ����

int main(int argc, char *argv[]){
	int fd1, fd2, count;
	char buf[MAX_BUF];

	if(argc !=3 ){ //argv[1]�� file�� argv[2] file�� �����ϱ� ���� argc == 3�̾�� �ϱ⿡ �˻�
		printf("Usage: %s source destination\n", argv[0]);
		exit(1); //error�� ���� ���� ����
	}

	if((fd1 = open(argv[1], O_RDONLY)) < 0){ //argv[1]�� file�� read������ open, descriptot�� fd1�� ����(3�� ����� ��)
		perror("open"); //open ���� �� error �޽��� ����
		exit(1);
	}
	
	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){ //ragv[2]�� file�� write������ open, copy�� ���� ���� ������ ��� �����, file�� ���ٸ� ����, mode�� 644
		perror("open");
		exit(1);
	}	
	
	while((count = read(fd1, buf, MAX_BUF)) > 0){ // count ������ MAX_BUFȤ�� fd1�� ���� ũ�⸸ŭ ����
		write(fd2, buf, count); //count��ŭ fd2�� write
	} //MAX_BUF��ŭ �ݺ��ؼ� write�ϴٰ� fd1�� ���� fileũ�Ⱑ MAX_BUF���� ������ �ش� ���� �κ��� write
	//���� end of file�� ������ 0�� �Ǹ� �ݺ� ����

	//file close
	close(fd1); close(fd2);

	return 0;
}
