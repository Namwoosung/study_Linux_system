#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{
	int x1;
	int x2;
	int y1;
	int y2;
	int result;
} numset;

void *CalSum(void *arg){
	numset *data = (numset *)arg;
	
	data->result = data->x1 * data->y1 + data->x2 * data->y2;
}

int main(){
	pthread_t tid1, tid2, tid3, tid4;
	int **arr1;
	int **arr2;
	
	numset data1;

	if(pthread_create(&tid1, NULL, CalSum, (void *)&data1) < 0){
		perror("phread)create");
		exit(1);
	} 

	return 0;
}
