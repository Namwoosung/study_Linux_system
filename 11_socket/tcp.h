#define MY_ID 15

#define SERV_TCP_PORT (7000 + MY_ID) //TCP port number는 모두 다른 것을 사용해야 함
#define SERV_HOST_ADDR "127.0.0.1" 
//127.0.0.1은 루프백 IP address, 자기 자신과 통신할 때 사용
//만약 다른 server와 통신하고 싶으면 자신의 server의 IP address를 적어줘야 함

#define MSG_REQUEST 1
#define MSG_REPLY 2

typedef struct{
	int type; //request 혹은 reply type
	char data[128];
} MsgType;
