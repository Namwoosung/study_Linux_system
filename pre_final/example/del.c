#include <stdio.h>
#include <string.h>

void del(char str[]){
	strcpy(str, (str + 1));
}


int main(){
	char str[20] = "Hello";
	del(str);
	puts(str);

	return 0;
}
