#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_SIZE 1024

char Pdata[MAX_SIZE]; //shared data
pthread_mutex_t Mutex; //mutex
pthread_cond_t Client;
pthread_cond_t Server;

void ClientThread(void *dummy){
	//lock을 얻고 진입, data를 전송하는 동작
	if(pthread_mutex_lock(&Mutex) < 0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	
	sprintf(Pdata, "This is a request from client.");
	printf("send a reqesut....");

	if(pthread_mutex_unlock(&Mutex) < 0){
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}
	if(pthread_cond_signal(&Client) < 0){
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	
	//이후 server에게 온 응답을 기다리다가 받고 출력
	if(pthread_mutex_lock(&Mutex) < 0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if(pthread_cond_wait(&Server, &Mutex) < 0){
		perror("pthread_cond_wait");
		pthread_exit(NULL);
	}
	printf("Received reply: %s\n", Pdata);
	if(pthread_mutex_unlock(&Mutex) < 0){
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

void ServerThread(void *dummy){
	//이후 server에게 온 응답을 기다리다가 받고 출력
	if(pthread_mutex_lock(&Mutex) < 0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if(pthread_cond_wait(&Client, &Mutex) < 0){
		perror("pthread_cond_wait");
		pthread_exit(NULL);
	}
	printf("Received request: %s......", Pdata);
	sprintf(Pdata, "This is a reply from server.");
	printf("Replied.\n");
	if(pthread_mutex_unlock(&Mutex) < 0){
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}
	if(pthread_cond_signal(&Server) < 0){
		perror("pthread_cond_wait");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

int main(){
	pthread_t tid1, tid2;

	if(pthread_cond_init(&Client, NULL) < 0){
		perror("pthread_cond_init");
		exit(1);
	}
	if(pthread_cond_init(&Server, NULL) < 0){
		perror("pthread_cond_init");
		exit(1);
	}
	if(pthread_mutex_init(&Mutex, NULL) < 0){
		perror("pthread_cond_init");
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
	
	if(pthread_cond_destroy(&Client) < 0){
		perror("pthread_cond_destroy");
	}
	if(pthread_cond_destroy(&Server) < 0){
		perror("pthread_cond_destroy");
	}
	if(pthread_mutex_destroy(&Mutex) < 0){
		perror("pthread_cond_destroy");
	}

	return 0;
}
