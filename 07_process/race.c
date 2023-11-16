#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void CharAtaTime(char *str){
	char *ptr;
	int c, i;
	
	setbuf(stdout, NULL); //stdout�� buffer�� NULL�� ������ ���۸����� �ʰ�, �ٷ� ����ϰ� ��
	for(ptr = str; c = *ptr++;){
		//�ݺ��� �ϴ� ������ race condition�� ����� �ֱ� ���� time quantum�� �����ϵ��� �ϴ� ��
		//child�� parent process�� ���� �ϳ��� ����ϰ�, �ݺ����� ���ٰ� time qunatum�� �����ϰ�, ready ���·� ���ư� ��
		for(i = 0; i < 999999; i++)
			;
		//���ڸ� �ϳ��� ���
		putc(c, stdout);
	}
}

int main(){
	pid_t pid;
	
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	//child process�� parent process�� ���� CharAtaTime �Լ��� ����
	//�Լ� ���ۿ� ���� ���ڸ� ����ϴٰ� time quantum�� ��� ������ ready�� ���ư��⸦ �ݺ��� ��
	//console�� shared data�� �Ǿ� ������ ���� ��µ� ��
	else if(pid == 0){
		CharAtaTime("output from child\n");
	}
	else{
		CharAtaTime("output from parent\n");
	}

	return 0;
}
