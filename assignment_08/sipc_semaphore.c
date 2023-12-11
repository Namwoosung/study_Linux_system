#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define MAX_SIZE 1024

char Pdata[MAX_SIZE]; //shared data
sem_t MutexSem, ServerSem, ClientSem; //semaphore

void ClientThread(void *dummy){
	//lock을 얻고 진입, data를 전송하는 동작
	if(sem_wait(&MutexSem) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	
	sprintf(Pdata, "This is a request from client.");
	printf("send a reqesut....");

	if(sem_post(&MutexSem) < 0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	if(sem_post(&ClientSem) < 0){
		perror("sem_post");
		pthread_exit(NULL);
	}

	
	//이후 server에게 온 응답을 기다리다가 받고 출력
	if(sem_wait(&ServerSem) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if(sem_wait(&MutexSem) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	printf("Received reply: %s\n", Pdata);
	if(sem_post(&MutexSem) < 0){
		perror("sem_post");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

void ServerThread(void *dummy){
	//이후 server에게 온 응답을 기다리다가 받고 출력
	if(sem_wait(&ClientSem) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if(sem_wait(&MutexSem) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("Received request: %s......", Pdata);
	sprintf(Pdata, "This is a reply from server.");
	printf("Replied.\n");

	if(sem_post(&MutexSem) < 0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	if(sem_post(&ServerSem) < 0){
		perror("sem_post");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

int main(){
	pthread_t tid1, tid2;

	if(sem_init(&MutexSem, 0, 1) < 0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&ServerSem, 0, 0) < 0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&ClientSem, 0, 0) < 0){
		perror("sem_init");
		exit(1);
	}

	if(pthread_create(&tid1, NULL, (void *)ServerThread, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)ClientThread, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}

	if(pthread_join(tid1, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	
	if(sem_destroy(&MutexSem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&ServerSem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&ClientSem) < 0){
		perror("sem_destroy");
	}

	return 0;
}
