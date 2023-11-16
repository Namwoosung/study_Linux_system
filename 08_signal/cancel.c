#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t ThreadId[2];
//SIGINT hnadler
void SigIntHandler(int signo){
	int i;
	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate other threads: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);
	//pthread_cancel로 생성한 thread를 kill
	for(i = 0; i < 2; i++){
		if(pthread_cancel(ThreadId[i])){
			perror("pthread_cancel");
			exit(1);
		}
	}
	//모두 terminate될 때까지 대기
	for(i = 0; i < 2; i++){
		if(pthread_join(ThreadId[i], NULL)){
			perror("pthread_join");
			exit(1);
		}
	}
	printf("Thread terminated: tid = %ld, %ld\n", ThreadId[0], ThreadId[1]);

	exit(0);
}

void Thread1(void *dummy){
	printf("Thread %ld created....\n", pthread_self());
	//thread의 cancel 옵션 저장
	//다른 곳에서 cancel 가능
	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)){
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	//asynchronous하게 바로 죽도록 설정(원래는 lock을 가지고 있거나 하면 반환 후 종료되어야 하기에 연기하는 것이 default지만, 예제에서는 의미 없으니 바로 죽도록 설정)
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)){
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}

	while(1)
		;

}

void Thread2(void *dummy){
	printf("Thread %ld created....\n", pthread_self());

	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)){
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)){
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}

	while(1)
		;
}

int main(){
	//thread를 2개 생성
	if(pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&ThreadId[1], NULL, (void *)Thread1, NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	//SIGINT handler 등록
	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for( ; ; )
		pause();

	return 0;
}
