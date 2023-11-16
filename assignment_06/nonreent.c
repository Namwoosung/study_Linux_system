#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void myAlarmHandler(int signo){
	struct passwd *rootptr; 

	//signal handler를 재등록하는 과정, 리눅스는 불필요
	signal(SIGALRM, myAlarmHandler);
	alarm(1);

	printf("is signal handler\n");
	//rootptr로 root의 정보를 가져옴
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
		//cjs의 정보를 ptr로 가져옴
		if((ptr = getpwnam("cjs")) == NULL){
			perror("getpwnam");
			exit(1);
		}
		//가져온 정보의 이름이 cjs가 아닐경우 synchronization problem이 발생했으므로 종료
		if(strcmp(ptr->pw_name, "cjs") != 0){
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}

	//main의 getpwnam이 진행중에 handler의 getpwnam이 호출되면, main에서 가져오는 정보가 변경되면서 동기화문제가 발생
	//getpwnam은 non reentrant function인데 signal handler로 사용되어서 문제 발생

	return 0;
}
