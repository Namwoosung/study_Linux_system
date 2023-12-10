#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

	"process ���� compile�ÿ��� LDFALGS = -lpthread�� ��������� ��"

int main(){
	//�� �� ����
	int pid;
	pid_t pid2;
	
	"fork ����"
	if((pid = fork()) < 0){
		"fork�� ���� �� child�� pid, Ȥ�� 0, ���н� -1 ��ȯ"
		perror("fork");
		exit(1);
	}else if(pid == 0){
		"child process"
	}else{
		"parent process"
	}
	
	
	"exit ����"
	if(atexit(myexit1) != 0){
		"exit���� �Լ��� ���� �� 0, ���� �� 0�� �ƴ� �� ��ȯ"
		perror("atexit");
		exit(1); "�Ķ���ʹ� status, parent process�� ���� exit ���¸� �˷��ִ� ����"
	}


	"wait ����"
	int status;
	pid_t pid;
	if((pid = fork()) < 0){}
	else if(pid == 0) {}
	else {
		wait(&status);
		"wait�� status�� �ּҸ� �Ѱ��ָ� child�� exit ���¸� Ȯ�� ����, �ʿ� ������ NULL"
		"wait�� ��ȯ ���� process ID�ε� ��� ��� x"
		"��� status�� ��ȯ�� �� �� Ư���� ��Ȳ�ƴϸ� �ȽἭ wait(NULL)�� �ַ� ���"
		if(waitpid(pid, &status, 0) < 0){
			"waitpid�� pid���� ����� �Ȱ���, ������ �ĸ����ʹ� �ɼ��ε� �׳� 0"
			perror("waitpid");
			exit(1)
		}
	}
	
	
	"exec ����"
	pid_t pid;
	char *argv[] = {"mycp, exec.c, exec2.c, NULL"};
	if((pid = fork()) < 0) {}
	else if(pid == 0){
		"exec ���� �Լ����� ������ ��ȯ ����, ���� �� -1 ��ȯ"
		"exec ù��° �Ķ���ʹ� ��Ȯ�� path, ���� �ڿ��� argv0 ���� null"
		if(execl("/home/lsp15/05_filrIO/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0){
			perror("execl");
			exit(1);
		}
		if(execv("/home/lsp15/05_fileIO/mycp", argv) < 0){
			perror("execv");
			exit(1);
		}
		if(execlp("env", "env", NULL) < 0){
			"execlp�� PATH���� ������ ã���� ��� ��� ���� filename, argv0���� NULL �Է�"
			perror("execlp");
			exit(1)
		}
		"�߰��� execle, execve�� ����° ���ڷ� ȯ�溯���� array���� �Ѱ���"
	}


	"ȯ�溯�� ����"
	int main(int argc, char *argv[], char *envp[]) "�Ķ���ͷ� ������"
	extern char **environ; "������ ��������" 
	for(i = 0; envp[i] != NULL; i++){
		printf("%s\n",envp[i]);
	}
	char **p;
	for(p = envp; *p != NULL; p++){
		printf("%s\n", *p);
	}


	"system ����"
	int status;
	if((status = system("date; who")) < 0){
		"system�� ���� �� ���� status, ���� �� -1 ��ȯ"
		perror("system");
		exit(1);
	}


	"thread ����"
	pthread_t tid1;
		"thread �Լ����� ���� �� 0��ȯ, ���� �� ���� ��ȯ"
	if(pthread_create(&tid1, NULL, (void *)function, (void *)arg) < 0){
		"ó�� �Ķ���Ϳ��� tid�� �޾ƿ���, 2��°�� attribute�ε� �׳� NULL������"
		"function�̶� �Ķ����, �Ķ���� �� �� ������ struct�� ����"
		perror("pthread_create");
		exit(1);
	}

	"pthread_exit(NULL)�� function ���� �������� ����Ǿ� ���� ��"
	
	if(pthread_join(tid1, NULL) < 0){
		"tid1�� ����� �� ���� wait"
		perror("pthread_join");
		exit(1);
	}	

	"structȰ��� �������� thread ���ô� assignment05/tadd.c ����"

	return 0;
}
