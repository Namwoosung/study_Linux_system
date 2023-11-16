#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
	pid_t pid;
	char *argv[] = {"mycp", "exec.c", "exec2.c", NULL};	//execv���� �Ķ���ͷ� �Ѱ��� list ����
	//execl
	if((pid = fork()) <0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){ //child process�� ���
		//child process�� execl�� �����ϸ鼭 mycp�� ���۽�Ű�� process�� ��ȭ
		//execl�� �Ķ���͸� ����Ʈ���·� �Ѱ���
		//ó���� �����ų ������ path ���� argv�� �Ѱ��� ��, �������� NULL�� ���
		if(execl("/home/lsp15/05_fileIO/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0 ){
			perror("execl");
			exit(1);
		}
	}
	//parent process�� ��� �ڽ��� ���������� ��ٸ�
	if(waitpid(pid, NULL, 0) < 0){
		perror("waitpid");
		exit(1);
	}
	
	//execv
	//�ٽ� ���ο� process ����
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		//child process�� execv�� ���� mycp ����, array���·� �Ķ���� ����
		if(execv("/home/lsp15/05_fileIO/mycp", argv) < 0){
			perror("execv");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0){
		perror("waitpid");
		exit(1);
	}
	//execlp
	//execlp�� ���α׷��� �����ų �� PATH���� Ž���ؼ� ����
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	else if(pid == 0){
		//env ���α׷��� PATH���� ã�Ƽ� �����Ű��� �ǹ�
		if(execlp("env", "env", NULL) < 0){
			perror("execlp");
			exit(1);
		}
	}
	if(waitpid(pid, NULL, 0) < 0 ){
		perror("waitpid");
		exit(1);
	}

	return 0;
}
