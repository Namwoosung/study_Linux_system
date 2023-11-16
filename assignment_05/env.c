#include <stdio.h>
//main함수의 세번째 파라미터로 envp를 넣어 환경변수를 envp변수에 저장
int main(int argc, char *argv[], char *envp[]){
	int i;
	char **p;
	//C start-up routine에서 선언된 environ 전역변수를 extern해서 선언
	extern char **environ;

	printf("List command-line arguments\n");
	for(i = 0; i < argc; i++){
		printf("%s\n", argv[i]);
	}
	
	printf("\n");
	printf("List environment variables from environ variable\n");

#if 1 //#if 1이기에 컴파일과정에서 해당구문이 compile됨, else 문을 compile하고 싶으면 if 0으로 설정
	//environ 전역변수의 내용을 출력
	for(i = 0; environ[i] != NULL; i++){
		printf("%s\n", environ[i]);
	}
#else
	//string으로 가져와서 출력
	for(p = environ; *p != NULL; p++){
		printf("%s\n", *p);
	}
#endif

	printf("\n");
	printf("List environment variables form envp varialbe\n");

#if 1
	//main의 파라미터로 받아오느 envp 내용을 출력
	for(i = 0; envp[i] != NULL; i++){
		printf("%s\n", envp[i]);
	}
#else
	//string으로 받아와서 출력
	for(p = envp; *p != NULL; p++){
		printf("%s\n", *p);
	}
#endif

	return 0;
}
