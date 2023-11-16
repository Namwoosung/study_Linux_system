#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(){
	struct stat statbuf;

	if(stat("bar", &statbuf) < 0){
		perror("stat");
		exit(1);
	}
	//bar�� mode�� ����, ���� ��忡�� group write�� ���� ���� S_ISUID�� on
	if(chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID ) < 0){
		perror("chmod");
		exit(1);
	}
	//foo�� mode�� ����, ���� ��� ������� ���� mode��� ����
	if(chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0){
		perror("chmod");
		exit(1);
	}

	return 0;
}
