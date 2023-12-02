//deadlock을 방지한 예제, starvation은 발생 가능
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_MEN 5
#define NLOOPS 5

#define LEFT(i) ((i + NUM_MEN-1) % NUM_MEN)
#define RIGHT(i) ((i+1)%NUM_MEN)

enum {THINKING, EATING, HUNGRY};

sem_t Philosopher[NUM_MEN]; //semaphore가 철학자마다 존재
sem_t Mutex; //shared data인 State에 대한 Mutex
int State[NUM_MEN]; //철학자 각각의 state


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

//자신이 밥을 먹고 싶고, 양 옆이 EATING이 아니라면 lock을 얻음
void Test(int id){
	if(State[id] == HUNGRY && State[LEFT(id)] != EATING && State[RIGHT(id)] != EATING){
		State[id] = EATING;

		if(sem_post(&Philosopher[id]) < 0){
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

void PickUp(int id){
	//shared data 보호
	if(sem_wait(&Mutex) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	//자신의 상태를 HUNGRY로 바꾸고 먹을 수 있는 지 test
	State[id] = HUNGRY;
	Test(id);
	if(sem_post(&Mutex) < 0){
		perror("sem_post");
		pthread_exit(NULL);
	}
	//test에서 lock을 얻었다면 진행가능, 만약 양 옆에 먹는 사람이 있었다면 여기서 대기
	if(sem_wait(&Philosopher[id]) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

void PutDown(int id){
	if(sem_wait(&Mutex) < 0){
		perror("sem_wait");
		pthread_exit(NULL);
	}
	//다 먹었으므로 THINKING으로 상태를 변경
	State[id] = THINKING;
	//만약 나를 기다리는 양 옆 사람이 존재할 수 있으니 test를 진행
	Test(LEFT(id));
	Test(RIGHT(id));

	if(sem_post(&Mutex) < 0){
		perror("sem_post");
		pthread_exit(NULL);
	}
}

void DiningPhilosopher(int *pId){
	int i;
	int id = *pId;

	for(i = 0; i < NLOOPS; i++){
		Thinking(id);
		PickUp(id);
		Eating(id);
		PutDown(id);
	}

	printf("Philosopher%d: thinking & eating %d times....\n", id, i);

	pthread_exit(NULL);
}

int main(){
	pthread_t tid[NUM_MEN];
	int i, id[NUM_MEN];

	srand(0x8888);
	//철학자마다 semaphore 생성, 일단 모두 먹을 수 없는 상태로 생성
	for(i = 0; i < NUM_MEN; i++){
		if(sem_init(&Philosopher[i], 0, 0) < 0){
			perror("sem_init");
			exit(1);
		}	
		id[i] = i;
	}
	if(sem_init(&Mutex, 0, 1) < 0){
		perror("sem_init");
		exit(1);
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
		if(sem_destroy(&Philosopher[i]) < 0){
			perror("sem_destroy");
		}
	}

	return 0;
}
