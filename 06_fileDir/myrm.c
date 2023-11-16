#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc == 1){
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}
	int i;
	for(i = 1; i < argc; i++){ //argv[1]부터 argv[argc -1]까지 모든 파일에 대해 반복
		if(remove(argv[i]) < 0){ //각 파일을 remove하고, 실패 시 error
			perror("remove");
			exit(1);
		}

	}
	return 0;
}
