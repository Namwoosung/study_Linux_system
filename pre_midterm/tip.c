#include <stdio.h>
#include <stdlib.h>

int main(){
	"�����Ҵ�"
	"ũ�⸦ �Է¹޾� �ش� ũ���� int array ����"
	int value;
	scanf("%d", &value);
	int *numPtr = (int *)malloc(sizeof(int) * value);
	free(numPtr);


	"write�Լ��� printf ����"
	char buf[] = "a write to stdout\n";
	write(STDOUT_FILENO, buf, sizeof(buf) - 1);


	"sleep�Լ��� ��� �Ķ���� ��ŭ �� ������ �޽�"
	sleep(2);


	"setbuf�� buffer ũ�⸦ ���Ƿ� �������ִ� �ǵ� ���� �� �� ���� ������.."
	setbuf(stdout, NULL);


	"fputc(c, stdout)�̶� putchar(c)�� ����"	

	

	"compile ���ǹ�"
#if 1
	"��� compile"
#else
	"��� compile x"
#endif
	


	"string integer convert"
	char str[] = "10";
	int i = atoi(str);

	char str[80];
	int i = 10;
	sprintf(str, "%d", i);
	sprintf(str, "%#X", 0x9f);
	sprintf(str, "%f", 3.141592);
	

	"ASCII code ��"
	A = 65, Z = 80
	a = 97, Z = 122

	return 0;
}
