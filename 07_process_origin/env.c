#include <stdio.h>
//main�Լ��� ����° �Ķ���ͷ� envp�� �־� ȯ�溯���� envp������ ����
int main(int argc, char *argv[], char *envp[]){
	int i;
	char **p;
	//C start-up routine���� ����� environ ���������� extern�ؼ� ����
	extern char **environ;

	printf("List command-line arguments\n");
	for(i = 0; i < argc; i++){
		printf("%s\n", argv[i]);
	}
	
	printf("\n");
	printf("List environment variables from environ variable\n");

#if 1 //#if 1�̱⿡ �����ϰ������� �ش籸���� compile��, else ���� compile�ϰ� ������ if 0���� ����
	//environ ���������� ������ ���
	for(i = 0; environ[i] != NULL; i++){
		printf("%s\n", environ[i]);
	}
#else
	//string���� �����ͼ� ���
	for(p = environ; *p != NULL; p++){
		printf("%s\n", *p);
	}
#endif

	printf("\n");
	printf("List environment variables form envp varialbe\n");

#if 1
	//main�� �Ķ���ͷ� �޾ƿ��� envp ������ ���
	for(i = 0; envp[i] != NULL; i++){
		printf("%s\n", envp[i]);
	}
#else
	//string���� �޾ƿͼ� ���
	for(p = envp; *p != NULL; p++){
		printf("%s\n", *p);
	}
#endif

	return 0;
}
