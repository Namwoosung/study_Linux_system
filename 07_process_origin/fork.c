#include <stdio.h>
#include <sys/types.h> //for typedef
#include <unistd.h> //for fork function

//���������� Var ����
int Var = 6;
char Buf[] = "a write to stdout\n";

int main(){
	//���������� var ����
	int var;
	pid_t pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf) - 1); //Buf ������ ���
	printf("Before fork\n");

	if((pid = fork()) == 0){ //child process�� ��� fork()�� ��ȯ���� 0
		/*child*/
		//child porcess������ ������ �� +1
		Var++;
		var++;
	}
	else{ //parent process�� ��� fork()�� ��ȯ���� child process�� PID
		/*parent*/
		sleep(2);
	}
	//parent process���� fork�� �����ϸ� child process�� ����
	//���� ���������� parent�� child�� ������ ���� ����(child�� parent�� �����ؼ� ����)
	//���Ŀ��� ���� �ٸ� memory�� ����ϱ⿡ �������� process�� ����
	//chuld�� if ���� �� ���� ����, �׷��� parent�� else������ �� ���� ���� x
	//���� scheduling algorithm�� ���� child�� parent �� �����ϰ� ����
	//parent�� sleep(2)�̱⿡ parent process���� ��� �� ���� parent process�� ���
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);

	return 0;
}
