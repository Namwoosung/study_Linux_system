#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void myAlarmHandler(int signo){
	struct passwd *rootptr; 

	//signal handler�� �����ϴ� ����, �������� ���ʿ�
	signal(SIGALRM, myAlarmHandler);
	alarm(1);

	printf("is signal handler\n");
	//rootptr�� root�� ������ ������
	if((rootptr = getpwnam("root")) == NULL){
		perror("getpwnam");
		exit(1);
	}
	
	return ;
}

int main(){
	struct passwd *ptr;
	
	signal(SIGALRM, myAlarmHandler);
	alarm(1);
	
	for( ; ; ){
		//cjs�� ������ ptr�� ������
		if((ptr = getpwnam("cjs")) == NULL){
			perror("getpwnam");
			exit(1);
		}
		//������ ������ �̸��� cjs�� �ƴҰ�� synchronization problem�� �߻������Ƿ� ����
		if(strcmp(ptr->pw_name, "cjs") != 0){
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}

	//main�� getpwnam�� �����߿� handler�� getpwnam�� ȣ��Ǹ�, main���� �������� ������ ����Ǹ鼭 ����ȭ������ �߻�
	//getpwnam�� non reentrant function�ε� signal handler�� ���Ǿ ���� �߻�

	return 0;
}
