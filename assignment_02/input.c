#include <stdio.h>

int main(){
	char c, s[80];
	int i; long l;
	float f; double d;

	//scanf와 printf는 format string을 활용
	//C는 reference type이 존재하지 않기에, pointer 형식으로 파라미터를 넘겨 값을 주소에 적을 수 있도록 함
	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d);
	printf("%c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d);

	c = getchar(); //character는 입력으로 받는 함수
	putchar(c); //character 출력

	gets(s); //string을 입력으로 받는 함수
	puts(s); //string 출력

	return 0;
}
