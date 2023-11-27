//동기화 x, busy-waiting하는 multi-thread 예제
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

//multi thread환경이므로 전역변수가 shared data역할
BoundedBufferType Buf;

//thread를 micro second 단위로 sleep 시키기 위한 함수
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


void Producer(void *dummy){
	int i, data;
	
	printf("Producer: Start....\n");
	
	//NLOOPS만큼 반복하며 Buf에 wirte
	for(i = 0; i < NLOOPS; i++){
		//Buf가 가득찼으면 무한반복하며 busy-waiting
		if(Buf.counter == MAX_BUF){
			printf("Producer: Buffer full. Waiting....\n");
			while(Buf.counter == MAX_BUF)
				;
		}
		
		printf("Producer: Producing an item....\n");
		//임의의 data 생성
		data = (rand() % 100) * 10000;
		//현재 넣어야 할 index에 data 저장
		Buf.buf[Buf.in].data = data;
		//index와 counter를 +1
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;
		
		//random한 시간만큼 sleep
		ThreadUsleep(data);
	}

	printf("Producer: Produced %d items....\n", i);
	printf("Producer: %d items in buffer....\n", Buf.counter);

	pthread_exit(NULL);
}

//Producer와 유사하게 동작, Buf에서 read를 반복
void Consumer(void *dummy){
	int i, data;

	printf("Consumer: Start....\n");

	for(i = 0; i < NLOOPS; i++){
		if(Buf.counter == 0){
			printf("Consumer: Buffer empty. Waiting....\n");
			while(Buf.counter == 0)
				;
		}
		
		printf("Consumer: Consuming an item....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		ThreadUsleep((rand() % 100) * 10000);
	}

	printf("Consumer: Consumed %d items....\n", i);
	printf("Consumer: %d items in buffer....\n", Buf.counter);

	pthread_exit(NULL);
}

int main(){
	pthread_t tid1, tid2;

	srand(0x8888);
	//producer와 consumer thread를 각각 생성
	if(pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	//2개의 thread가 종료될 때 까지 wait
	if(pthread_join(tid1, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}

	printf("Main	: %d items in buffer....\n", Buf.counter);

	return 0;
}
