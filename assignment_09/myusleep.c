#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

void myusleep(int secs, int usecs){
	struct timeval tm;
	
	tm.tv_sec = secs;
	tm.tv_usec = usecs;
	
	//select인데 socket을 기다리지 않음
	//그냥 tm만큼 기다리도록 구현 -> sleep을 select로 구현
	//여기서 tm 구조체는 상대시간
	if(select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0){
		perror("select");
		return ;
	}

}

int main(){
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);

	return 9;
}
