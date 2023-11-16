#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{
	int start;
	int end;
	int result;
} Sumdata;

//thread���� ������ sum �Լ�
void *mySum(void *arg){
	int i;
	//Sumdata ����ü�� �޾ƿ�
	Sumdata *data = (Sumdata *)arg;
	//start���� end���� ��� ���� �Ķ���ͷ� ���� struct�� result�� ����� ����
	data->result = 0;
	for(i = data->start; i <= data->end; i++){
		data->result += i;
	}
	pthread_exit(NULL);
}

int main(){
	//thread id�� ������ thread���� ����� Sumdata ����
	pthread_t tid1, tid2;
	Sumdata data1, data2;

	data1.start = 1;
	data1.end = 50;
	data2.start = 51;
	data2.end = 100;
	//2���� thread�� ������ ���� ������ ����
	if(pthread_create(&tid1, NULL, mySum, (void *)&data1) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, mySum, (void *)&data2) < 0){
		perror("pthread_create");
		exit(1);
	}
	//2���� thread�� ���������� wait
	if(pthread_join(tid1, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) <0){
		perror("pthread_join");
		exit(1);
	}
	//thread�� ����� ���������� sum
	int sum = data1.result + data2.result;
	
	printf("threads terminated, result: %d\n", sum);

	return 0;
}
