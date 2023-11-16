#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}
	//argv[1]�� file name�� argv[2]�� ����, ���� �� error
	if(rename(argv[1], argv[2]) < 0){
		perror("rename");
		exit(1);
	}

	//���� mv�� rename��ɰ� file move ����� ��� ����������, mymv�� ��� rename ��ɸ� ����	

	return 0;
}
