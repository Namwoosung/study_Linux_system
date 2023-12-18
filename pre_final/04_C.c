	"input and output format string"

#include <stdio.h>
#include <string.h>

int main(){

	char c = 'a', s[] = "hello";
	int i = 100; long l = 99999;
	float f = 3.14; double d = 99.999;
	int *p = &i;

	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p);
	putchar(c);
	puts(s);



	char c, s[80];
	int i; long l;
	float f; double d;

	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d);
	printf("%c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d);

	c = getchar();
	putchar(c);

	gets(s);
	puts(s);

	return 0;
}

	"string function ���� ����, (�����δ� #include <string.h>�ؼ� ���̺귯�� �Լ� ���)"
int strlen_p(char *str){
	int len = 0;
	//str pointer�� ���� null�� �� ���� �̵���Ű�鼭 len���� ����
	while(*str++){
		len++;	
	}
	return len;
}

int strlen_a(char str[]){
	int i;
	//str�迭�� ���� null�� �� ���� �̵���Ű�鼭 i�� ����
	for(i = 0; str[i] != '\0'; i++)
		;
	return i;
}

void strcpy_p(char* dst, char*src){
	while(*src){ //src�� string�� null�� ������ �ݺ�
		*dst++ = *src++; //src�� ���� dst�� �����ϰ� pointer�� ��ĭ �� �̵�
	}
	*dst = *src; //������ null character ����
}

void strcpy_a(char dst[], char src[]){
	int i;
	for(i = 0; src[i] != '\0'; i++){
		dst[i] = src[i];
	}
	dst[i] = src[i];
}

void strcat_p(char* dst, char* src){
	while(*dst++){ //dst string�ڿ� src string�� �߰��ϱ� ���� dst pointer�� ��ġ�� null�� �̵�
		;
	}
	dst--; //���� �����ڿ� ���� null ���� ��ġ�� �̵��Ǿ����� null��ġ�� �̵���Ű�� ���� �� ĭ �ڷ� �̵�
	while(*src){ //src�� null�� �� ����
		*dst++ = *src++; //dst�� src�� �� ����
	}
	*dst = *src; //������ null�� ����
}

void strcat_a(char dst[], char src[]){
	int i, j;
	for(i = 0; dst[i] != '\0'; i++) ; //dst pointer ��ġ �̵�
	for(j = 0; src[j] != '\0'; j++){
		dst[i + j] = src[j]; //�� ����
	}
	dst[i+j] = src[j]; //null�� ����
}

int strcmp_p(char* dst, char*src){
	while( *dst == *src ){ //dst�� src�� �����ϸ� ��� Ž���ϸ鼭 �ݺ�
		if( *dst == '\0') return 0; //���� �� string�� null�� �����ϸ� ��� ������ ���̹Ƿ� 0 return
		dst++;
		src++;
	}

	return (*dst - *src); //dst�� src�� �ٸ��ٸ� ASCII ���� �� return
}

int strcmp_a(char dst[], char src[]){
	int i = 0;
	while(dst[i] == src[i]){ //������ ��� ��� �˻�
		if(dst[i] == '\0') return 0; //null�̸� 0 reutrn
		i++; 
	}

	return (dst[i] - src[i]); //ASCII���� �� return
}	

int main(){
	int len1, len2;
	char str1[20], str2[20];

	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p = %d, a = %d\n", len1, len2);

	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p = %s, a= %s\n", str1, str2);

	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p = %s, a = %s\n\n", str1, str2);
	
	printf("testcase for strcmp\n");
	int a, b, c, d;
	strcpy_p(str1, "abcd");
	strcpy_p(str2, "abc");
	//abcd�� abc�� ���̹Ƿ� a�� b�� positive value�� ���̰�, d�� ASCII���� 100�� ����� ��
	a = strcmp_p(str1, str2);
	b = strcmp_a(str1, str2);
	printf("compare abcd and abc\nstrcmp: p = %d, a = %d\n", a, b);
	strcpy_p(str1, "A");
	strcpy_p(str2, "a");
	//A�� a�� ���ε�, ASCII�󿡼� �ҹ��ڰ� �� ũ�� ������ ������ ��ȯ�� ��
	a = strcmp_p(str1, str2);
	b = strcmp_a(str1, str2);
	printf("compare A and a\nstrcmp: p = %d, a = %d\n", a, b);
	strcpy_p(str1, "AaBb");
	strcpy_p(str2, "AaBb");
	//�� ���ڿ��� �����ϱ⿡ 0�� ��ȯ�� ��
	a = strcmp_p(str1, str2);
	b = strcmp_a(str1, str2);
	printf("compare AaBb  and AaBb\nstrcmp: p = %d, a = %d\n", a, b);

	return 0;
}
