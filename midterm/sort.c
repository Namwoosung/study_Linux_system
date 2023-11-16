#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "usage fault");
		exit(1);	
	}
	
	FILE *fp, *dst;
	int ch;
	int data[10];

	if((fp = fopen(argv[1], "rt")) == NULL){
		perror("fopen");
		exit(1);
	}
	
	int i = 0;
	int j = 0;

	while(i < 10){
		ch = fgetc(fp);
		if(ch == ' ')
			continue;
		data[i] = ch;
		i++;	
	}

	int temp = 0;
	for(i = 0; i < 10; i++){
		for(j = i; j < 10; j++){
			if(data[i] > data[j]){
				temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}
	}

	if((dst = fopen(argv[2], "wt")) == NULL){
		perror("fopen");
		exit(1);
	}
	
	for(i = 0; i < 10; i++){
		fputc(data[i], dst);
		fputc(' ', dst);
	}
	
	fputc('\0', dst);

	fclose(fp);
	fclose(dst);

	return 0;
}
