#include <stdio.h>

int main(){
	
	"stat 사용법"
	struct stat statbuf;
	if(lstat("~/06_fileDir/stat.c", &statbuf) < 0){
		"stat관련 함수는 성공 시 0, 실패 시 음수 반환"
		"처음 파라미터에 filepath나 argv[i] 대입"
		"fstat은 file descriptor 대입"
		perror("lstat");
		exit(1);
	}
	
	if(S_ISREG(statbuf.st_mode)) "macro로 file type 식별 가능"


	"umask 사용법"
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0) {}

	"chmod 사용법"
	if(chmod("foo", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0){
		"성공 시 0, 실패 시 -1"
		"fie name대신 descriptor를 받는 fchmod도 존재"
		perror("chmod");
		exit(1);
	}


	"link 생성"
	"성공 시 0, 실패 시 -1"
	if(link(argv[1], argv[2]) < 0){
		"argv[1]으로 argv[2] 생성"
		perror();
		exit()
	}
	if(symlink(argv[1], argv[2]) < 0) {}


	"remove"
	if(remove(argv[i]) < 0) {}
		"성공 0 실패 -1"

	"rename"
	if(rename(argv[1], argv[2]) < 0){
		"성공 0 실패 -1"
	}

	"mkdir"
	if(mkdir(argv[1], 0755) < 0) {}

	"rmdir"
	if(rmdir(argv[1]) < 0) {}


	"directory reading 함수"
	DIR *dp;
	struct dirent *dep;
	struct stat statbuf;
	char fullpath[MAX_PATH];
	
	if((dp = opendir(path)) == NULL) {}
	"성공시 dir의 pointer, 실패 시 NULL"
	while(dep = readdir(dp)){
		"file의 pointer 반환, 끝 도달시 NULL return"
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);
		"dep에서 사실상 사용할 것은 d_name밖에 없음"
	}	
	rewinddir(dp);
	closedir(dp);
	"recursive하게 dir내의 dir까지 검사하는 코드는 06_fileDir/mylsr.c 참조"i


	"chdir과 getpwd"
#define MAX_BUF 256
	char buf[MAX_BUF];
	if(getcwd(buf, MAX_BUF) == NULL){
		"성공시 buf에 위치 저장, 실패 시 NULL"
	}
	if(chdir(argv[1]) < 0){
		"성공 시 0, 실패 시 -1"
	}

	"mylsl에서 활용한 UID, GID, date 등 세부 내용은 assignment_04/mylsl.c 참조"
	
	return 0;
}
