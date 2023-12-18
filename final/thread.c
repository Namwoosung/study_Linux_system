#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

char Msg[80];

sem_t EmptySem, FullSem, MutexSem;

void In(void *dummy){
	strcpy(Msg, "\0");
	while(strcmp(Msg,"quit") != 0){
		if(sem_wait(&EmptySem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if(sem_wait(&MutexSem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		printf("In> ");
		fflush(stdout);
		fgets(Msg, 80, stdin);
		*strchr(Msg, '\n') = '\0';
		if(sem_post(&MutexSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
		if(sem_post(&FullSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	pthread_exit(NULL);
}

void Out(void *dummy){
	int i;
	while(strcmp(Msg, "QUIT") != 0){
		if(sem_wait(&FullSem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if(sem_wait(&MutexSem) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		for(i = 0; i < 80; i++){
			if(Msg[i] == '\0') break;
			if(!isupper(Msg[i])) Msg[i] -= 32;
		}
		printf("OUT> %s\n", Msg);
	
		if(sem_post(&MutexSem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
		if(sem_post(&EmptySem) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	pthread_exit(NULL);

}

int main(){
	pthread_t tid1, tid2;

	if(sem_init(&EmptySem, 0, 1) < 0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&FullSem, 0, 0) < 0){
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&MutexSem, 0, 1) < 0){
		perror("sem_init");
		exit(1);
	}
	
	if(pthread_create(&tid1, NULL, (void *)In, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)Out, (void *)NULL) < 0){
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

	if(sem_destroy(&EmptySem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&FullSem) < 0){
		perror("sem_destroy");
	}
	if(sem_destroy(&MutexSem) < 0){
		perror("sem_destroy");
	}

	return 0;
}

