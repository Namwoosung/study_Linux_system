#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int Var;

int main(){
	Var = 1;
	//�ʱ� ���μ����� pid�� ����
	pid_t pid = getpid();
	
	//�ʱ� process���� �ݺ����� �� ������ ���� �����ϴ� process��ŭ�� process�� ������
	//�� �ݺ��� ���� Var *2�� ���ָ� ���� process ���� ��� ����
	for(int i = 0; i < 4; i++){
		fork();
		Var *= 2;
	}

	//�ʱ� process�� ������ Var ���� ��ü ������ process ����
	if(pid == getpid()){
		printf("%d\n", Var);
	}

	return 0;
}
