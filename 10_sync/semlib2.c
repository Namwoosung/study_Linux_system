#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"

int sem_init(sem_t *sem, int pshared, int value){
	//pshared는 사용하지 않는 파라미터이므로 있으면 -1
	if(pshared){
		fprintf(stderr, "Function not supported\n");
		return -1;
	}
	//구조체의 변수들 초기화
	if(pthread_mutex_init(&sem->mutex, NULL) < 0){
		return -1;
	}
	if(pthread_cond_init(&sem->cond, NULL) < 0){
		return -1;
	}
	
	sem->sval = value;
	
	return 0;
}

int sem_wait(sem_t *sem){
	//sem의 sval이 shared data이므로 mutex로 보호
	if(pthread_mutex_lock(&sem->mutex) < 0){
		return -1;
	}
	
	//semaphore의 값이 0이면
	if(sem->sval == 0){
		while(sem->sval == 0){
			//condition varible을 활용해 기다리기
			if(pthread_cond_wait(&sem->cond, &sem->mutex) < 0){
				//이건 예외처리하는 부분, 크게 의미 x
				if(pthread_mutex_unlock(&sem->mutex) < 0)
					return -1;
				return -1;
			}
		}
		//이후에 빠져나왔다면 sval -1
		sem->sval--;
	}
	else{
		//semaphore가 0이 아니라면 -1
		sem->sval--;
	}

	if(pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;
	return 0;
}

//wait과 유사한데 semaphore가 0이면 바로 -1 retunr하고 wait하지 않음
int sem_trywait(sem_t * sem){
	if(pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if(sem->sval == 0){
		if(pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	else{
		sem->sval--;
	}
	
	if(pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int sem_post(sem_t *sem){
	if(pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if(sem -> sval == 0){
		//기다리던 cond를 깨워줌
		if(pthread_cond_signal(&sem->cond) < 0){
			if(pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}
	}

	sem->sval++;
	
	if(pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}


int sem_getvalue(sem_t *sem, int *sval){
	*sval = sem->sval;

	return 0;
}

int sem_destroy(sem_t *sem){
	if(pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;
	if(pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}

