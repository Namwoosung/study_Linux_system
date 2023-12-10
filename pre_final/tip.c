#include <stdio.h>
#include <stdlib.h>

int main(){
	"동적할당"
	"크기를 입력받아 해당 크기의 int array 생성"
	int value;
	scanf("%d", &value);
	int *numPtr = (int *)malloc(sizeof(int) * value);
	free(numPtr);


	"write함수로 printf 구현"
	char buf[] = "a write to stdout\n";
	write(STDOUT_FILENO, buf, sizeof(buf) - 1);


	"sleep함수는 잠시 파라미터 만큼 초 단위로 휴식"
	sleep(2);


	"setbuf는 buffer 크기를 임의로 설정해주는 건데 거의 쓸 일 없지 않을까.."
	setbuf(stdout, NULL);


	"fputc(c, stdout)이랑 putchar(c)는 동일"	

	

	"compile 조건문"
#if 1
	"얘는 compile"
#else
	"얘는 compile x"
#endif
	


	"string integer convert"
	char str[] = "10";
	int i = atoi(str);

	char str[80];
	int i = 10;
	sprintf(str, "%d", i);
	sprintf(str, "%#X", 0x9f);
	sprintf(str, "%f", 3.141592);
	

	"ASCII code 값"
	A = 65, Z = 80
	a = 97, Z = 122

	return 0;
}
