#include <stdio.h>
#include <string.h>

void strupper(char str[]){
	while(*str){
		if(*str >= 97 && *str <= 122){
			*str -= 32;
		}
		str++;
	}
}

int main(){
	char test[] = "hello, WoRld";
	printf("%s\n", test);
	strupper(test);
	printf("%s\n", test);

	return 0;
}
