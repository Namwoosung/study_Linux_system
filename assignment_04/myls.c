#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	DIR *dp;
	struct dirent *dep;

	if((dp = opendir(".")) == NULL){ //현재 directory를 열고 dp에 pointer 저장
		perror("opendir");
		exit(0);
	}
	while(dep = readdir(dp)){ //dp에서 각 dirent를 순서대로 dep에 저장, 끝에 도달하면 NULL을 반환하며 종료
		printf("%s\n", dep->d_name); //각 dep의 name을 출력
	}
	
	closedir(dp);
	return 0;
}
