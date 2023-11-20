#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(long)

int update(long *ptr){
	return ((*ptr)++);
}

int main(){
	int fd, i, counter;
	pid_t pid;
	caddr_t area;
	
	// /dev/zero를 open
	// zero file은 unix에 존재하는 특별한 파일, 여기에 write하면 null이 채워지고, 여기서 read하면 그 만큼 null을 반환받음
	// 비슷하게 /dev/null file도 존재, null file은 내용이 아예 사라지게 됨
	if((fd = open("/dev/zero", O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	// zero file을 mapping해서 area pointer로 반환
	if((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (caddr_t)-1){
		perror("mmap");
		exit(1);
	}
	// 실제로 zero file에 대한 read, write를 하는 것은 아니기에 close
	close(fd);
	
	//parent와 child의 동기화를 위한 pipe를 생성 -> pipe를 통해 동기화 효과를 얻음
	//현재 예제에서는 shared memory를 생성해서, 해당 위치에 long type 변수를 생성
	//parent와 child가 번갈아가며 변수를 +1 하며 shared memory를 공유하는 예제
	TELL_WAIT();
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}else if (pid > 0){
		//parent process
		for(i = 0; i < NLOOPS; i += 2){
			//초기 area 위치에는 0이 저장 -> counter는 0이 될 것
			//이후 child가 +1을 하고 다시 parent가 실행, 다음 시행에서 counter는 2가될 것
			//즉 counter와 i는 동일한 값으로 설정될 것이고, 그렇지 않으면 mismatch이므로 종료
			if((counter = update((long *)area)) != i){
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			//자신이 할 일을 다 했으니 child를 awake
			TELL_CHILD();
			//자신은 child의 시행이 끝날 때 까지 대기
			WAIT_CHILD();
		}
	}else{
		//child process
		for(i = 1; i < NLOOPS; i += 2){
			//parent의 시행을 대기
			WAIT_PARENT();
			//동일하게 area의 변수를 counter로 받고 area의 변수를 +1
			if((counter = update((long *) area)) != i){
				fprintf(stderr, "Counter mismach\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			//자신이 시행이 끝났으니 parent를 awake
			TELL_PARENT();
		}
	}

	return 0;
}
