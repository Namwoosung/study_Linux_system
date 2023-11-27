#define MY_ID 15
#define SHM_KEY (0x9000 + MY_ID)
#define SHM_MODE (SHM_R | SHM_W | IPC_CREAT)

#define EMPTY_SEM_KEY (0x5000 + MY_ID)
#define FULL_SEM_KEY (0x6000 + MY_ID)
#define MUTEX_SEM_KEY (0x7000 + MY_ID)

#define NLOOPS 20

#define MAX_BUF 2

//buffer�� ����Ǵ� item�� struct, ���ÿ����� data�ϳ��� ���������� �����δ� �� ID, name, ... ������ ���� ����
typedef struct{
	int data;
} ItemType;

//buffer struct
typedef struct{
	//����� MAX_BUF�� 2�̹Ƿ� buffer���� 2���� ������� ���� -> ItemType�� 2�� ���尡��
	ItemType buf[MAX_BUF];
	//in�� out�� ���� write�� read�� index
	int in;
	int out;
	//���� buffer�� ����Ǿ� �ִ� item�� ����
	int counter;
} BoundedBufferType;

//shared data�� Buf�̹Ƿ� shared memory�� ����� buffer�� ũ��� ����
#define SHM_SIZE sizeof(BoundedBufferType)
