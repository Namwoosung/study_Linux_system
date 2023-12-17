#define MY_ID 15

#define SERV_TCP_PORT (7000 + MY_ID) //TCP port number�� ��� �ٸ� ���� ����ؾ� ��
#define SERV_HOST_ADDR "127.0.0.1" 
//127.0.0.1�� ������ IP address, �ڱ� �ڽŰ� ����� �� ���
//���� �ٸ� server�� ����ϰ� ������ �ڽ��� server�� IP address�� ������� ��

#define MSG_REQUEST 1
#define MSG_REPLY 2

typedef struct{
	int type; //request Ȥ�� reply type
	char data[128];
} MsgType;
