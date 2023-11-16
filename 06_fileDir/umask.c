#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h> //for creat function

int main(){
	umask(0); //umask 설정 x
	if(creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0){ //설정한 mode대로 file creat
		perror("creat");
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ); //umaks 설정
	if(creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0){ //umask로 설정된 bit는 off된 채로 mode가 설정된 후 creat
		perror("creat");
		exit(1);
	}
	return 0;
}
