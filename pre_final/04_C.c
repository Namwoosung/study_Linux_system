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

	"string function 직접 구현, (실제로는 #include <string.h>해서 라이브러리 함수 사용)"
int strlen_p(char *str){
	int len = 0;
	//str pointer의 값이 null일 때 까지 이동시키면서 len값을 증가
	while(*str++){
		len++;	
	}
	return len;
}

int strlen_a(char str[]){
	int i;
	//str배열의 값이 null일 때 까지 이동시키면서 i값 증가
	for(i = 0; str[i] != '\0'; i++)
		;
	return i;
}

void strcpy_p(char* dst, char*src){
	while(*src){ //src의 string이 null일 때까지 반복
		*dst++ = *src++; //src의 값을 dst에 대입하고 pointer를 한칸 씩 이동
	}
	*dst = *src; //마지막 null character 대입
}

void strcpy_a(char dst[], char src[]){
	int i;
	for(i = 0; src[i] != '\0'; i++){
		dst[i] = src[i];
	}
	dst[i] = src[i];
}

void strcat_p(char* dst, char* src){
	while(*dst++){ //dst string뒤에 src string을 추가하기 위해 dst pointer의 위치를 null로 이동
		;
	}
	dst--; //후위 연산자에 의해 null 다음 위치로 이동되었으니 null위치로 이동시키기 위해 한 칸 뒤로 이동
	while(*src){ //src가 null일 때 까지
		*dst++ = *src++; //dst에 src의 값 대입
	}
	*dst = *src; //마지막 null값 대입
}

void strcat_a(char dst[], char src[]){
	int i, j;
	for(i = 0; dst[i] != '\0'; i++) ; //dst pointer 위치 이동
	for(j = 0; src[j] != '\0'; j++){
		dst[i + j] = src[j]; //값 대입
	}
	dst[i+j] = src[j]; //null값 대입
}

int strcmp_p(char* dst, char*src){
	while( *dst == *src ){ //dst와 src가 동일하면 계속 탐색하면서 반복
		if( *dst == '\0') return 0; //만약 두 string이 null로 동일하면 모두 동일한 것이므로 0 return
		dst++;
		src++;
	}

	return (*dst - *src); //dst와 src가 다르다면 ASCII 차이 값 return
}

int strcmp_a(char dst[], char src[]){
	int i = 0;
	while(dst[i] == src[i]){ //동일한 경우 계속 검사
		if(dst[i] == '\0') return 0; //null이면 0 reutrn
		i++; 
	}

	return (dst[i] - src[i]); //ASCII차이 값 return
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
	//abcd와 abc의 비교이므로 a와 b는 positive value일 것이고, d의 ASCII값인 100이 저장될 것
	a = strcmp_p(str1, str2);
	b = strcmp_a(str1, str2);
	printf("compare abcd and abc\nstrcmp: p = %d, a = %d\n", a, b);
	strcpy_p(str1, "A");
	strcpy_p(str2, "a");
	//A와 a의 비교인데, ASCII상에서 소문자가 더 크기 때문에 음수가 반환될 것
	a = strcmp_p(str1, str2);
	b = strcmp_a(str1, str2);
	printf("compare A and a\nstrcmp: p = %d, a = %d\n", a, b);
	strcpy_p(str1, "AaBb");
	strcpy_p(str2, "AaBb");
	//두 문자열이 동일하기에 0이 반환될 것
	a = strcmp_p(str1, str2);
	b = strcmp_a(str1, str2);
	printf("compare AaBb  and AaBb\nstrcmp: p = %d, a = %d\n", a, b);

	return 0;
}
