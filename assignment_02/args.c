//command���� �ĸ����ͷ� �Ѿ�� file �̸��� ����ϴ� ���α׷�
#include <stdio.h>

int main(int argc, char *argv[]){
	int i;
	for(i = 0; i < argc; i++){ //�Ķ������ ���� argc�� ����Ǳ⿡ �Ķ������ �� ��ŭ �ݺ�
		printf("argv[%d] = %s\n", i, argv[i]); //������� argv�� ����� file �̸��� ���
	}

	return 0;
}
