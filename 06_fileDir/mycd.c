#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}
	if(chdir(argv[1]) < 0) { //parameter로 넘어온 경로로 change directory 시행
		perror("chdir"); //실패시 error
		exit(1);
	}
	//mycd를 실제로 실행하면 파라미터로 넘겨준 path로 이동하지 않음
	//그 이유는 mycd를 실행하면 새로운 process가 fork되어 생성되고 해당 process에서 change directory가 이루어지기 때문
	//chdir 위와 아래에 getcwd를 시행하면 directory가 성공적으로 이동한 것을 확인 가능
	//그러나 shell의 directory에는 영향을 주지 않고, 새로운 process만 변경되기에 directory가 변경되지 않는 것	


	return 0;
}
