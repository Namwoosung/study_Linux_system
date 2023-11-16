#include <stdio.h>
#include <sys/types.h> //for typedef
#include <dirent.h> //for DIR finctions
#include <sys/stat.h> //for lstat function
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 256

void JustDoIt(char *path){
	DIR *dp;
	struct dirent *dep;
	struct stat statbuf;
	char fullpath[MAX_PATH];	

	if((dp = opendir(path)) == NULL){ //�Ķ������ path�� directory open
		perror("opendir");
		exit(0);
	}
	printf("\n%s:\n", path);
	while(dep = readdir(dp)){ //���ʷ� dirent ����ü�� dep�� ����
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) //.�� ..�� ��� ������� �ʰ� pass
			continue;
		printf("%s\n", dep->d_name); //���
	}
	
	rewinddir(dp); //directory�� �ٽ� �˻��ϱ� ���� rewind
	while(dep = readdir(dp)){
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name); //���� �˻��ϴ� dirent�� ��ü path�� fullpath�� ����
		if(lstat(fullpath, &statbuf) < 0){ //�ش� path�� file Ȥ�� directory�� i-node�� lstat���� ������
			perror("lstat");
			exit(1);
		}
		if(S_ISDIR(statbuf.st_mode)) //���� dirent�� directory�� ���
			JustDoIt(fullpath); //recursive�ϰ� function�� ȣ���� �ش� directory�� entry�� ���
	}

	closedir(dp);
}

int main(int argc, char *argv[]){
	JustDoIt("."); //���� directory�� path�� �Ѱܼ� function ����
	return 0;
}
