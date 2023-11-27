#define MY_ID 15
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)

#define EMPTY_SEM_KEY (0x5000 + MY_ID)
#define FULL_SEM_KEY (0x6000 + MY_ID)
#define MUTEX_SEM_KEY (0x7000 + MY_ID)

#define NLOOPS 20

#define MAX_BUF 2

//buffer에 저장되는 item의 struct, 예시에서는 data하나만 존재하지만 실제로는 고객 ID, name, ... 등으로 구현 가능
typedef struct{
	int data;
} ItemType;

//buffer struct
typedef struct{
	//현재는 MAX_BUF가 2이므로 buffer에는 2개의 저장공간 존재 -> ItemType을 2개 저장가능
	ItemType buf[MAX_BUF];
	//in과 out은 현재 write와 read할 index
	int in;
	int out;
	//현재 buffer에 저장되어 있는 item의 개수
	int counter;
} BoundedBufferType;

//shared data는 Buf이므로 shared memory의 사이즈를 buffer의 크기로 지정
#define SHM_SIZE sizeof(BoundedBufferType)
