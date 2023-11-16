#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int fd[2];
	pid_t child;

	if(pipe(fd) < 0 ){
		perror("pipe");
		exit(1);
	}
	child = fork();
	if(child == (pid_t)(-1)){
		perror("fork");
		exit(1);
	} else if(child == (pid_t)(0)){
		//STDOUT�� read�� pipe close
		close(1);
		close(fd[0]);
		//write�� pipe�� fd[1]�� close�� file descriptor �� ���� ���� ���ڸ� duplicate
		//��������� STROU�� duplicate
		//���� �ش� process���� STDOUT���� ��µǴ� ���� pipe�� write�� ��
		if(dup(fd[1]) == -1){
			perror("dup");
			exit(1);
		}
		//ls -l����, PATH���� ��ɾ ã�Ƽ� ����
		//������ ls -l�� ����� STDOUT���� ��µ�����, ����� fd[1]���� ���
		if(execlp("ls", "ls", "-l", NULL) == -1){
			perror("execlp");
			exit(1);
		}

	} else{
		//STDIN�� write�� pipe close
		close(0);
		close(fd[1]);
		//fd[0]�� STDIN duplicate
		if(dup2(fd[0], 0) == -1){
			perror("dup");
			exit(1);
		}
		//more�� ����
		//�������� STDIN���� ���� ���� more�ؼ� ���
		//����� pipe�� read�ؼ� ����� ��
		if(execlp("more", "more", NULL) == -1){
			perror("execlp");
			exit(1);
		}
	}

	return 0;
}
