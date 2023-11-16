#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc == 1){
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}
	int i;
	for(i = 1; i < argc; i++){ //argv[1]���� argv[argc -1]���� ��� ���Ͽ� ���� �ݺ�
		if(remove(argv[i]) < 0){ //�� ������ remove�ϰ�, ���� �� error
			perror("remove");
			exit(1);
		}

	}
	return 0;
}
