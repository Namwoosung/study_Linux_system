//동기화 x, busy-waiting하는 multi-process 예제
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

int main(){
	BoundedBufferType *pBuf;
	int shmid, i, data;
	//shared memory 생성
	if((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0){
		perror("shmget");
		exit(1);
	}
	//memory에 attach
	if((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *)-1){
		perror("shmat");
		exit(1);
	}
	//NLOOPS만큼 반복하며 read, random 시간만큼 쉬면서 반복
	srand(0x9999);
	for(i = 0; i < NLOOPS; i++){
		if(pBuf->counter == 0){
			printf("Consumer: Buffer empty. Waiting....\n");
			while(pBuf->counter == 0)
				;
		}
		printf("Consumer: Consuming an item....\n");
		data = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;

		usleep((rand() % 100) * 10000);
	}
	printf("Consumer: Consumed %d items....\n", i);
	printf("Consumer: %d items in buffer....\n", pBuf->counter);

	return 0;
}
