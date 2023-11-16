#include <stdio.h>
#include <unistd.h> //for link function
#include <stdlib.h>

int main(int argc, char *argv[]){
	
	if(argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if(link(argv[1], argv[2]) < 0){ //argv[1]�� hard link�� argv[2]�� ����, ���н� error
		perror("link");
		exit(1);
	}
	return 0;
}
