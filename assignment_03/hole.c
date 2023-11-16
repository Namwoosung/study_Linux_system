#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int fd;
	char ch = '\0';	

	if((fd = creat("file.hole", 0400)) < 0){ //file.hole ���� ����, mode�� 0400, fd���� 3�� ����� ��
		perror("creat"); //�Լ� return ���� -1�̶�� �����̹Ƿ� error�޽����� creat ����
		exit(1); //��������
	}
	
	if(lseek(fd ,1023, SEEK_SET) < 0){ //lseek�� ���� file�� �����͸� ó����ġ ���ؿ��� 1023 byte��ŭ �̵�
		perror("lseek"); //���н� error ����
		exit(1);
	}
	write(fd, &ch, 1); //1024��ġ�� null�� write, ��������� 1024byte������ �����ϸ鼭 ��� ������ ����ִ� file ����

	close(fd);

	return 0;
}
