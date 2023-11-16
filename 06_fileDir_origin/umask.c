#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h> //for creat function

int main(){
	umask(0); //umask ���� x
	if(creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0){ //������ mode��� file creat
		perror("creat");
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ); //umaks ����
	if(creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0){ //umask�� ������ bit�� off�� ä�� mode�� ������ �� creat
		perror("creat");
		exit(1);
	}
	return 0;
}
