#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void CharAtaTime(char *str){
	char *ptr;
	int c, i;
	
	setbuf(stdout, NULL);
	for(ptr = str; c = *ptr++;){
		for(i = 0; i < 999999; i++)
			;
		putc(c, stdout);
	}
}

int main(){
	pid_t pid;
	
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		CharAtaTime("output from child\n");
	}
	else{
		wait(NULL); //parent process�� wait�� �߰�
		//parent process�� child process�� ����Ǳ⸦ ��ٷȴٰ� ���� �� ��
		//status�� ������� ������ NULL �Է�
		CharAtaTime("output from parent\n");
	}

	return 0;
}
