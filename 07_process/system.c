#include <stdio.h>
#include <stdlib.h>

int main(){
	int status;
	//system function�� string���� command �Է��� �޾� �������ִ� �Լ�
	//���������� fork, exec�� �����ϰ�, parent�� waitpid�� ��ٸ����� ����
	//status�� termination status
	if((status = system("date")) < 0){
		perror("sytem");
		exit(1);
	}
	printf("exit status = %d\n", status);
	//�ش� command�� �������� ������ error�� �߻��� ��
	if((status = system("nosuchcommand")) < 0 ){
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
	//�������� command�� ;�� �����ؼ� ���� ����
	if((status = system("who; exit 44")) <0){
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
	
	return 0;
}
