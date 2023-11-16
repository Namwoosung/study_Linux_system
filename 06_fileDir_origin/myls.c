#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	DIR *dp;
	struct dirent *dep;

	if((dp = opendir(".")) == NULL){ //���� directory�� ���� dp�� pointer ����
		perror("opendir");
		exit(0);
	}
	while(dep = readdir(dp)){ //dp���� �� dirent�� ������� dep�� ����, ���� �����ϸ� NULL�� ��ȯ�ϸ� ����
		printf("%s\n", dep->d_name); //�� dep�� name�� ���
	}
	
	closedir(dp);
	return 0;
}
