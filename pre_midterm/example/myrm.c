#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	while(argc > 0){
		argc--;
		if(remove(argv[argc]) < 0){
			perror("remove");
			exit(1);
		}
	}

	return 0;
}
