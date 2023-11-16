#include <stdio.h>
#include <stdlib.h>
//exit handler�� ����� �Լ� 2�� ����
void myexit1(){
	printf("first exit handler\n");
}

void myexit2(){
	printf("second exit handler\n");
}

int main(){
	//myexit2�� exit handler�� ����, ���� �� 0, ���� �� �ٸ� �� ��ȯ�̹Ƿ� 0���� Ȯ��
	if(atexit(myexit2) != 0){
		perror("atexit");
		exit(1);
	}

	if(atexit(myexit1) != 0){
		perror("atexit");
		exit(1);
	}

	if(atexit(myexit1) != 0){
		perror("atexit");
		exit(1);
	}
	
	printf("main is done\n");
	//main���� return�Ǹ鼭 C start-up routine�� ���� ���� exit function�� ����
	//exit function���� exit handler�� ����� �Լ����� ���ʷ� ����
	//������ function�� ������  ��ϰ���
	return 0;
}
