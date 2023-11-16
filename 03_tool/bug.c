#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	int i;
	double j;
	char *bug = (char*)malloc(sizeof(char) * 20);

	for(i = 0; i < 5; i++){
		j = i/2 + i;
		printf("j is %lf \n", j);
	}

	strcpy(bug, "hi");
	printf("bug is %s \n", bug);
	
	free(bug);
	return 0;
}
