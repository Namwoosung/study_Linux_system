#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char * argv[]){
	if(argc != 3){
		fprintf(stderr, "usage fault");
		exit(1);	
	}
	
	FILE *fp;
	int ch;
	int data[10];

	if((fp = fopen(argv[1], "rt")) == NULL){
		perror("fopen");
		exit(1);
	}

	int i = 0;
	while(i < 10){
		ch = fgetc(fp);
		if(ch == ' ')
			continue;
		data[i] = ch - 48;
		i++;
	}

	int start, end, mid;
	int target = atoi(argv[2]);

	start = 0;
	end = 9;
	
	int pos = 0;

	while(start <= end){
		mid = (start + end) / 2;
		if(data[mid] < target){
			start = mid + 1;
		} else if(data[mid] > target){
			end = mid - 1;
		}
		else{
			printf("%d\n", mid);
			pos = 1;
			break;
		}
	}

	if(!pos){
		printf("None\n");
	}

	fclose(fp);

	return 0;
}
