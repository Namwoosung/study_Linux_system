#include <stdio.h>

int main(){
	
	"stat ����"
	struct stat statbuf;
	if(lstat("~/06_fileDir/stat.c", &statbuf) < 0){
		"stat���� �Լ��� ���� �� 0, ���� �� ���� ��ȯ"
		"ó�� �Ķ���Ϳ� filepath�� argv[i] ����"
		"fstat�� file descriptor ����"
		perror("lstat");
		exit(1);
	}
	
	if(S_ISREG(statbuf.st_mode)) "macro�� file type �ĺ� ����"


	"umask ����"
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0) {}

	"chmod ����"
	if(chmod("foo", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0){
		"���� �� 0, ���� �� -1"
		"fie name��� descriptor�� �޴� fchmod�� ����"
		perror("chmod");
		exit(1);
	}


	"link ����"
	"���� �� 0, ���� �� -1"
	if(link(argv[1], argv[2]) < 0){
		"argv[1]���� argv[2] ����"
		perror();
		exit()
	}
	if(symlink(argv[1], argv[2]) < 0) {}


	"remove"
	if(remove(argv[i]) < 0) {}
		"���� 0 ���� -1"

	"rename"
	if(rename(argv[1], argv[2]) < 0){
		"���� 0 ���� -1"
	}

	"mkdir"
	if(mkdir(argv[1], 0755) < 0) {}

	"rmdir"
	if(rmdir(argv[1]) < 0) {}


	"directory reading �Լ�"
	DIR *dp;
	struct dirent *dep;
	struct stat statbuf;
	char fullpath[MAX_PATH];
	
	if((dp = opendir(path)) == NULL) {}
	"������ dir�� pointer, ���� �� NULL"
	while(dep = readdir(dp)){
		"file�� pointer ��ȯ, �� ���޽� NULL return"
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);
		"dep���� ��ǻ� ����� ���� d_name�ۿ� ����"
	}	
	rewinddir(dp);
	closedir(dp);
	"recursive�ϰ� dir���� dir���� �˻��ϴ� �ڵ�� 06_fileDir/mylsr.c ����"i


	"chdir�� getpwd"
#define MAX_BUF 256
	char buf[MAX_BUF];
	if(getcwd(buf, MAX_BUF) == NULL){
		"������ buf�� ��ġ ����, ���� �� NULL"
	}
	if(chdir(argv[1]) < 0){
		"���� �� 0, ���� �� -1"
	}

	"mylsl���� Ȱ���� UID, GID, date �� ���� ������ assignment_04/mylsl.c ����"
	
	return 0;
}
