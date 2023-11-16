#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{
	int start;
	int end;
	int result;
} Sumdata;

//thread에서 수행할 sum 함수
void *mySum(void *arg){
	int i;
	//Sumdata 구조체를 받아옴
	Sumdata *data = (Sumdata *)arg;
	//start부터 end까지 모두 더해 파라미터로 받은 struct의 result에 결과값 저장
	data->result = 0;
	for(i = data->start; i <= data->end; i++){
		data->result += i;
	}
	pthread_exit(NULL);
}

int main(){
	//thread id와 각각의 thread에서 사용할 Sumdata 정의
	pthread_t tid1, tid2;
	Sumdata data1, data2;

	data1.start = 1;
	data1.end = 50;
	data2.start = 51;
	data2.end = 100;
	//2개의 thread를 생성해 각각 덧셈을 수행
	if(pthread_create(&tid1, NULL, mySum, (void *)&data1) < 0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, mySum, (void *)&data2) < 0){
		perror("pthread_create");
		exit(1);
	}
	//2개의 thread가 끝날때까지 wait
	if(pthread_join(tid1, NULL) < 0){
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, NULL) <0){
		perror("pthread_join");
		exit(1);
	}
	//thread의 결과를 최종적으로 sum
	int sum = data1.result + data2.result;
	
	printf("threads terminated, result: %d\n", sum);

	return 0;
}
