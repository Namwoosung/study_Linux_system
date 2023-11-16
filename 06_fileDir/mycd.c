#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}
	if(chdir(argv[1]) < 0) { //parameter�� �Ѿ�� ��η� change directory ����
		perror("chdir"); //���н� error
		exit(1);
	}
	//mycd�� ������ �����ϸ� �Ķ���ͷ� �Ѱ��� path�� �̵����� ����
	//�� ������ mycd�� �����ϸ� ���ο� process�� fork�Ǿ� �����ǰ� �ش� process���� change directory�� �̷������ ����
	//chdir ���� �Ʒ��� getcwd�� �����ϸ� directory�� ���������� �̵��� ���� Ȯ�� ����
	//�׷��� shell�� directory���� ������ ���� �ʰ�, ���ο� process�� ����Ǳ⿡ directory�� ������� �ʴ� ��	


	return 0;
}
