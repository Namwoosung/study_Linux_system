//prodcons ������ mutex�� Ȱ���� ����
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType Buf;
//shared data�� ��ȣ�� Mutex�� full�� empty event�� ���� condition variable ����
pthread_cond_t NotFull;
pthread_cond_t NotEmpty;
pthread_mutex_t Mutex;

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
	
	printf("Producer: Start...\n");

	for(i = 0; i < NLOOPS; i++){
		//shared data�� buf�� �����ϱ� �� lock ����, �̹� lock�̸� ���
		if(pthread_mutex_lock(&Mutex) < 0){
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}
		//���� buf�� �� �������� wait
		//���⼭ while�� ������ ���� producer�� consumer�� ���� ���� ��Ȳ���� broadcast�� �����ָ� ����ϴ� thread�� ��� ���
		//�ϳ��� shared data�� �����ϰ� �������� �ٽ� wait�ϰ� �ϱ� ���� if�� �ƴ� while�� �˻�
		while(Buf.counter == MAX_BUF){
			//NotFull cond�� signal �� �� ���� ���, ���⼭ �ڽ��� �����ϴ� Mutex�� unlock
			if(pthread_cond_wait(&NotFull, &Mutex) < 0){
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Producer: Producing an item....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;
		//NotEmpty�� ��ٸ��� thread�� �����ϸ� signal ����
		if(pthread_cond_signal(&NotEmpty) < 0){
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}
		//shared data�� ��� ��������� unlock
		if(pthread_mutex_unlock(&Mutex) < 0){
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}
		ThreadUsleep(data);
	}
	printf("Producer: Produced %d items...\n", i);
	printf("Producer: %d items in buffer....\n", Buf.counter);

	pthread_exit(NULL);
}


void Consumer(void *dummy){
	int i, data;
	
	printf("Consumer: Start...\n");

	for(i = 0; i < NLOOPS; i++){
		if(pthread_mutex_lock(&Mutex) < 0){
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}
	
		while(Buf.counter == 0){
			if(pthread_cond_wait(&NotEmpty, &Mutex) < 0){
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Consumer: Consuming an item....\n");
		data = Buf.buf[Buf.out].data;	
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		if(pthread_cond_signal(&NotFull) < 0){
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}
		if(pthread_mutex_unlock(&Mutex) < 0){
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}
		ThreadUsleep(data);
	}
	printf("Consumer: Consumed %d items...\n", i);
	printf("Consumer: %d items in buffer....\n", Buf.counter);

	pthread_exit(NULL);
}

int main(){
	pthread_t tid1, tid2;

	srand(0x8888);
	//mutex�� cond init
	if(pthread_cond_init(&NotFull, NULL) < 0){
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if(pthread_cond_init(&NotEmpty, NULL) < 0){
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if(pthread_mutex_init(&Mutex, NULL) < 0){
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	if(pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0){
		perror("pthread_create");
		exit(1);
	}
	//thread ���� ���
	if(pthread_join(tid1, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}

	printf("Main	:%d items in buffer....\n", Buf.counter);
	//mutex �� cond ����
	if(pthread_cond_destroy(&NotFull) < 0){
		perror("pthread_cond_destroy");
	}
	if(pthread_cond_destroy(&NotEmpty) < 0){
		perror("pthread_cond_destroy");
	}
	if(pthread_mutex_destroy(&Mutex) < 0){
		perror("pthread_mutex_destroy");
	}

	return 0;
}
