#include <stdio.h>
#include <sys/types.h> //stat 구조체 변수의 type을 포함한 다양한 typedef
#include <sys/stat.h> //for stat, fstat, lstat function
#include <stdlib.h>

int main(int argc, char *argv[]){
	struct stat statbuf;
	char *mode;
	int i;

	for(i = 1; i < argc; i++){
		printf("%s: ",argv[i] ); //command line의 파라미터 순서대로 stat정보 출력
		if(lstat(argv[i], &statbuf) < 0){ //lstat으로 stat정보를 statbuf에 저장
			perror("lstat");
			continue;
		}
		//macro를 통해 해당 file의 type의 true, false를 검사해 mode에 저장
		if(S_ISREG(statbuf.st_mode))
			mode = "regular";
		else if(S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if(S_ISCHR(statbuf.st_mode))
			mode = "character special";
		else if(S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if(S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";
		else if(S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if(S_ISSOCK(statbuf.st_mode))
			mode = "socket";
		//mode와 stat정보를 출력
		printf("%s\n", mode);
		printf("\tst_mode = %d\n", statbuf.st_mode);
		printf("\tst_ino = %lu\n", statbuf.st_ino);
		printf("\tst_dev = %lu\n", statbuf.st_dev);
		printf("\tst_rdev = %lu\n", statbuf.st_rdev);
		printf("\tst_nlink = %lu\n", statbuf.st_nlink);
		printf("\tst_uid = %d\n", statbuf.st_uid);
		printf("\tst_gid = %d\n", statbuf.st_gid);
		printf("\tst_size = %ld\n", statbuf.st_size);
		printf("\tst_atime = %ld\n", statbuf.st_atime);
		printf("\tst_mtime = %ld\n", statbuf.st_mtime);
		printf("\tst_ctime = %ld\n", statbuf.st_ctime);
		printf("\tst_blksize = %ld\n", statbuf.st_blksize);
		printf("\tst_blocks = %ld\n", statbuf.st_blocks);
	}
	return 0;
}
