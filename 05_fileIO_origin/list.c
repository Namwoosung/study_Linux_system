//command line���� �ĸ����ͷ� �� file ������ line number�� �Բ� ����ϴ� ���α׷�
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024 //read�� MAX_BUF ����

int main(int argc, char *argv[]){
	FILE *fp;
	char buf[MAX_BUF];
	int line;

	if(argc != 2){ //�Ķ���ͷ� file�� �Ѿ���� ������ erroró��
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	if((fp = fopen(argv[1], "rt")) == NULL){ //text file�� read������ open, ���н� erroró��
		perror("fopen");
		exit(1);
	}

	line = 1;
	while(fgets(buf, MAX_BUF, fp)){ //file ������ line���� �о� buf�� ����
		printf("%4d: %s", line++, buf); //line number�� file ���� ���
	}//���� file �� ���޽� fgets�Լ��� NULL�� return�ϸ� �ݺ� ����

	fclose(fp);

	return 0;
}
