#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>



int main(void){
		
		FILE* file;
		if((file = fopen("q3java.txt", "w+")) == NULL){
			fprintf(stderr,"open error\n");
			exit(1);
		}
		/****** 두번째 매개변수 ******/
		/****** true : 기존 파일의 내용 이후부터 쓰여짐 ******/
		/****** false : 처음부터 쓰여짐 ******/

		fprintf(file, "%s" ,"2019 OSLAB\n");
		fprintf(file, "%s" ,"Linux System Programming\n");
		fflush(stdin);
		printf("DONE\n");

		fclose(file);
		
	}

