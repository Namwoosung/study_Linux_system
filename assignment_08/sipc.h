//sipc1과 sipc2에서 활용하기 위한 헤더파일
//shared memory를 위한 define
#define MY_ID 15
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_SIZE 1024
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)

//semaphore를 위한 define
#define EMPTY_SEM_KEY (0x5000 + MY_ID)
#define FULL_SEM_KEY (0x6000 + MY_ID)
#define MUTEX_SEM_KEY (0x7000 + MY_ID)
