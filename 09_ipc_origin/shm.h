//MY_ID는 shared memory의 key가 다른 user와 겹치치 않도록 자신의 number를 설정
#define MY_ID 15
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_SIZE 1024
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)
