#include <stdio.h>

int main(){
	char c, s[80];
	int i; long l;
	float f; double d;

	//scanf�� printf�� format string�� Ȱ��
	//C�� reference type�� �������� �ʱ⿡, pointer �������� �Ķ���͸� �Ѱ� ���� �ּҿ� ���� �� �ֵ��� ��
	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d);
	printf("%c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d);

	c = getchar(); //character�� �Է����� �޴� �Լ�
	putchar(c); //character ���

	gets(s); //string�� �Է����� �޴� �Լ�
	puts(s); //string ���

	return 0;
}
