#include <stdio.h>
#include <unistd.h>

#define MAX_CMD 256
//cmd�� ������ �����ϴ� ���� �ƴ϶� 1�� ���鼭 �����ϴ� ��ó�� ���̵��� �����ϴ� �Լ�
void DoCmd(char *cmd){
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main(){
	char cmd[MAX_CMD];
	
	//q�Է� ������ ���� �ݺ�
	while(1){
		printf("CMD> ");
		//console���� cmd�� �Է����� ����
		fgets(cmd, MAX_CMD, stdin);
		if(cmd[0] == 'q')
			break;
		DoCmd(cmd);
	}
	//�ش� ���α׷��� ���� process�� ����ǵ��� �����Ǿ��⿡ ����ڰ� cmd�� �Է��ϸ�
	//�ش� �Է¿� ���� ó���� ���� �ڿ� �ٽ� CMD>�� ����ϸ鼭 ����� ��
	return 0;
}
