#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
/*
#define THREAD_MAIN
*/
#define THREAD_1
//#define THREAD_2


void SigIntHandler(int signo){
	//signal을 받았을 때 어떤 thread에서 받았는 지 출력후 종료
	printf("Received a SIGINT signal by thread%ld\n", pthread_self());
	printf("Terminate this process\n");
	//리눅스에서는 signal은 main thread가 받아서 처리하기에 어떤 thread가 등록했는지 관계없이 main thread가 ISGINT를 받음

	exit(0);
}

//define여부에 따라 signal handler 등록 thread를 변경
void Thread1(void *dummy){
#ifdef THREAD_1
	signal(SIGINT, SigIntHandler);
#endif
	while(1)
		;
}

void Thread2(void *dummy){
#ifdef THREAD_2
	signal(SIGINT, SigIntHandler);
#endif
	while(1)
		;
}

int main(){
	pthread_t tid1, tid2;

	if(pthread_create(&tid1, NULL, (void *)Thread1, NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)Thread2, NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	//thread를 2개 만들고, 각각의 tid를 출력
	printf("Create two thread: tid1=%ld, tid2=%ld\n", tid1, tid2);
	printf("Main thread: tid=%ld\n", pthread_self());

#ifdef THREAD_MAIN
	signal(SIGINT, SigIntHandler);
#endif
	
	printf("press ^C to quit\n");

	for( ; ; )
		pause();

	return 0;
}
