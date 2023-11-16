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

	if((dp = opendir(path)) == NULL){ //파라미터의 path의 directory open
		perror("opendir");
		exit(0);
	}
	printf("\n%s:\n", path);
	while(dep = readdir(dp)){ //차례로 dirent 구조체를 dep에 저장
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) //.과 ..의 경우 출력하지 않고 pass
			continue;
		printf("%s\n", dep->d_name); //출력
	}
	
	rewinddir(dp); //directory를 다시 검사하기 위해 rewind
	while(dep = readdir(dp)){
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name); //현재 검사하는 dirent의 전체 path를 fullpath에 저장
		if(lstat(fullpath, &statbuf) < 0){ //해당 path의 file 혹은 directory의 i-node를 lstat으로 가져옴
			perror("lstat");
			exit(1);
		}
		if(S_ISDIR(statbuf.st_mode)) //현재 dirent가 directory일 경우
			JustDoIt(fullpath); //recursive하게 function을 호출해 해당 directory의 entry도 출력
	}

	closedir(dp);
}

int main(int argc, char *argv[]){
	JustDoIt("."); //현재 directory를 path로 넘겨서 function 시작
	return 0;
}
