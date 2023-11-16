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
	//bar의 mode를 변경, 현재 모드에서 group write를 지운 다음 S_ISUID를 on
	if(chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID ) < 0){
		perror("chmod");
		exit(1);
	}
	//foo의 mode를 변경, 현재 모드 관계없이 뒤의 mode대로 변경
	if(chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0){
		perror("chmod");
		exit(1);
	}

	return 0;
}
