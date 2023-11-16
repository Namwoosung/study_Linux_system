#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int mysystem(char *cmd){
	pid_t pid;
	int status;	
	
	//execv�� �Ķ���ͷ� �Ѱ��� argv array
	//system �Լ��� /bin/sh -c command�� ȣ���� ��ɾ ����, ���� shell�� ���� status�� ��ȯ�ϵ��� ����
	char* argv[] = {"/bin/sh", "-c", cmd, NULL};
	//fork ���� �� error �� -1 ��ȯ
	if((pid = fork()) < 0){
		perror("fork");
		return -1;
	}
	else if(pid == 0){
		//cmd command ����, ���� �� error �� ����
		if(execv("/bin/sh", argv) < 0){
			perror("execv");
			exit(1);
		}
	}
	//parent process�� child�� �����⸦ ��ٸ����� ����
	if(waitpid(pid, &status, 0) < 0){
		perror("waitpid");
		return -1;
	}
	//��ɾ��� exit code�� �˷��ִ� function
	return WEXITSTATUS(status);
}

//main �Լ����� mysystem�Լ� ��� ����
//system.c �ǽ� file�� ������ command���� ����
int main(){
	int status;

	if((status = mysystem("date")) < 0){
		perror("mysystem");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if((status = mysystem("nosuchcommand")) < 0){
		perror("mysystem");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if((status = system("who; exit 44")) <0){
		perror("mysystem");
		exit(1);
	}
	printf("exit status = %d\n", status);

	return 0;
}
