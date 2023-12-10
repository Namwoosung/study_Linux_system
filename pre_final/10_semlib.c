// System V semaphore를 POSIX 형태로 만들어 놓은 헤더파일
//실제 활용 시 assignment_08에 semlib.h와 semlib.c를 가져가서 활용

//	semlib.h
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semInit(key_t key);
int semInitValue(int semid, int value);
int semWait(int semid);
int semTryWait(int semid);
int semPost(int semid);
int semGetValeu(int semid);
int semDestroy(int semid);


//	semlib.c 
#include <stdio.h>
#include <unistd.h>
#include "semlib.h"

int semInit(key_t key){
	int semid;
	
	//semaphore를 하나만 생성
	if((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0){
		perror("semget");
		return -1;
	}

	return semid;
}

int semInitValue(int semid, int value){
	union semun{
		int val;
	} semun;

	semun.val = value;
	//semctl로 value를 초기화
	if(semctl(semid, 0, SETVAL, semun) < 0){
		perror("semctl");
		return -1;
	}
	return semid;
}

int semWait(int semid){
	struct sembuf semcmd;

	semcmd.sem_num = 0;
	semcmd.sem_op = -1;
	//SEM_UNDO는 process가 비정상적으로 종료됐을 경우 semaphore를 반납하고 종료하게 함
	semcmd.sem_flg = SEM_UNDO;
	if(semop(semid, &semcmd, 1) < 0){
		perror("semop");
		return -1;
	}
	return 0;
}

int semTryWait(int semid){
	struct sembuf semcmd;

	semcmd.sem_num = 0;
	semcmd.sem_op = -1;
	semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
	if(semop(semid, &semcmd, 1) < 0){
		perror("semop");
		return -1;
	}
	return 0;
}

int semPost(int semid){
	struct sembuf semcmd;

	semcmd.sem_num = 0;
	semcmd.sem_op = 1;
	semcmd.sem_flg = SEM_UNDO;
	if (semop(semid, &semcmd, 1) < 0){
		perror("semop");
		return -1;
	}
	return 0;
}

int semGetValue(int semid){
	union semun{
		int val;
	} dummy;
	return semctl(semid, 0, GETVAL, dummy);
}

int semDestroy(int semid){
	union semin{
		int val;
	} dummy;

	if(semctl(semid, 0, IPC_RMID, dummy) < 0){
		perror("semctl");
		return -1;
	}
	close(semid);
	return 0;
}
