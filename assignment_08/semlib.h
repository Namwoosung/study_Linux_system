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
