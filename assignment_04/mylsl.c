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

	//���� directory�� open
	if((dp = opendir(".")) == NULL){
		perror("opendir");
		exit(1);
	}
	while(dep = readdir(dp)){
		//���� directory�� �θ� directory ������ pass
		if(strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		//file path�� �����, file�� stat������ ������
		strcpy(fullpath, "./");
		strcat(fullpath, dep->d_name);
		if(lstat(fullpath, &statbuf) < 0){
			perror("lstat");
			exit(1);
		}
		//stat������ �������� file ���� ���
		//file type�� ���
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
		
		//file permission ���
		printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
		printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
		printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
		printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
		printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
		printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
		printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
		printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
		printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
		//statbuf�κ��� user name�� ������		
		pwd = getpwuid(statbuf.st_uid);
		//statbuf�κ��� group name�� ������
		g = getgrgid(statbuf.st_gid);
		//������ ���� ������ ������
		strftime(date, 20, "%b %d %H:%M", localtime(&statbuf.st_mtime));

		printf(" %ld %s %s %5ld %s %s\n", statbuf.st_nlink, pwd->pw_name, g->gr_name, statbuf.st_size, date, dep->d_name);
	}	

	return 0;
}
