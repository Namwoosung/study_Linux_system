#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
//thread ���� �� �Ѱ��� function
void PrintMsg(char *msg){
	printf("%s", msg);

	pthread_exit(NULL);
}

int main(){
	pthread_t tid1, tid2;
	char *msg1 = "Hello, ";
	char *msg2 = "World\n";
	//thread ����, id�� tid1�� ����, printMsg�� msg1 �Ķ���͸� �־ ����
	if(pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0){
		perror("pthread_create");
		exit(1);
	}

	printf("Threads created: tid = %ld, %ld\n", tid1, tid2);
	//tid1 thread�� ����� ������ wait
	if(pthread_join(tid1, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	
	printf("Threads terminated: tid = %ld, %ld\n", tid1, tid2);
	
	return 0;
}
