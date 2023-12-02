//deadlock이 발생할 가능성이 있는 예제
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_MEN 5
#define NLOOPS 5
//shared data로 젓가락이 존재
sem_t ChopStick[NUM_MEN];

void ThreadUsleep(int usecs){
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	struct timespec ts;
	struct timeval tv;

	if(pthread_cond_init(&cond, NULL) < 0){
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if(pthread_mutex_init(&mutex, NULL) < 0){
		perror("pthread_mutex_init");
		exit(1);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;

	if(ts.tv_nsec >= 1000000000){
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if(pthread_mutex_lock(&mutex) < 0){
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}	
	if(pthread_cond_timedwait(&cond, &mutex, &ts) < 0){
		perror("pthread_cond_timewait");
		pthread_exit(NULL);
	}

	if(pthread_cond_destroy(&cond) < 0){
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if(pthread_mutex_destroy(&mutex) < 0){
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

//임의의 시간만큼 sleep
void Thinking(int id){
	printf("Philosopher%d: Thinking....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat....\n", id);
}

//임의의 시간만큼 sleep
void Eating(int id){
	printf("Philosopher%d: Eating....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void DiningPhilosopher(int *pId){
	int i;
	int id = *pId;

	for(i = 0; i < NLOOPS; i++){
		//임의의 시간만큼 대기
		Thinking(id);
		//이후 2개의 lock을 순차적으로 얻음 -> 이 과정에서 deadlock발생가능, 모든 철학자가 왼쪽 젓가락을 들고 오른쪽 젓가락을 기다릴 수 있음
		if(sem_wait(&ChopStick[i]) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		if(sem_wait(&ChopStick[(i + 1) % NUM_MEN]) < 0){
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//2개의 lock을 얻었으면 식사
		Eating(id);
		//이후 2개를 순차적으로 signal
		if(sem_post(&ChopStick[i]) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
		if(sem_post(&ChopStick[(i + 1) % NUM_MEN]) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	printf("Philosopher%d: thinking & eating %d times....\n", id, i);

	pthread_exit(NULL);
}

int main(){
	pthread_t tid[NUM_MEN];
	int i, id[NUM_MEN];

	srand(0x8888);
	//5개의 semaphore 젓가락 생성
	for(i = 0; i < NUM_MEN; i++){
		if(sem_init(&ChopStick[i], 0, 1) < 0){
			perror("sem_init");
			exit(1);
		}	
		id[i] = i;
	}
	//철학자 5명마다 thread를 생성
	for(i = 0; i < NUM_MEN; i++){
		if(pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, (void *)&id[i]) < 0){
			perror("pthread_create");
			exit(1);
		}
	}
	//5개의 thread가 모두 종료되기를 wait
	for(i = 0; i < NUM_MEN; i++){
		if(pthread_join(tid[i], NULL) < 0){
			perror("pthread_join");
			exit(1);
		}
	}
	//semaphore를 삭제
	for(i = 0; i < NUM_MEN; i++){
		if(sem_destroy(&ChopStick[i]) < 0){
			perror("sem_destroy");
		}
	}

	return 0;
}
