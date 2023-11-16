#include <stdio.h>
#include <sys/types.h> //for typedef
#include <dirent.h> //for DIR function
#include <sys/stat.h> //for stat
#include <stdlib.h>
#include <string.h>
#include <pwd.h> //for UID
#include <grp.h> //for GID
#include <time.h> //for date

#define MAX_PATH 256

int main(int argc, char *argv[]){
	DIR* dp;
	struct dirent *dep;
	struct stat statbuf;
	char fullpath[MAX_PATH];
	struct passwd *pwd;
	struct group *g;
	char date[20];

	//현재 directory를 open
	if((dp = opendir(".")) == NULL){
		perror("opendir");
		exit(1);
	}
	while(dep = readdir(dp)){
		//현재 directory와 부모 directory 정보는 pass
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		//file path를 만들고, file의 stat정보를 가져옴
		strcpy(fullpath, "./");
		strcat(fullpath, dep->d_name);
		if(lstat(fullpath, &statbuf) < 0){
			perror("lstat");
			exit(1);
		}
		//stat정보를 바탕으로 file 정보 출력
		//file type을 출력
		if(S_ISREG(statbuf.st_mode))
			printf("-");
		else if(S_ISDIR(statbuf.st_mode))
			printf("d");
		else if(S_ISCHR(statbuf.st_mode))
			printf("c");
		else if(S_ISBLK(statbuf.st_mode))
			printf("b");
		else if(S_ISFIFO(statbuf.st_mode))
			printf("p");
		else if(S_ISLNK(statbuf.st_mode))
			printf("l");
		else if(S_ISSOCK(statbuf.st_mode))
			printf("s");
		
		//file permission 출력
		printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
		printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
		printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
		printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
		printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
		printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
		printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
		printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
		printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
		//statbuf로부터 user name을 가져옴		
		pwd = getpwuid(statbuf.st_uid);
		//statbuf로부터 group name을 가져옴
		g = getgrgid(statbuf.st_gid);
		//마지막 변경 시점을 가져옴
		strftime(date, 20, "%b %d %H:%M", localtime(&statbuf.st_mtime));

		printf(" %ld %s %s %5ld %s %s\n", statbuf.st_nlink, pwd->pw_name, g->gr_name, statbuf.st_size, date, dep->d_name);
	}	

	return 0;
}
