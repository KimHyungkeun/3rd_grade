#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ssu_time.h"

int main(int argc, char *argv[]) {

	struct timeval begin_t, end_t;
	struct stat file_info;
	char *str;
	int i;
	gettimeofday(&begin_t,NULL);

	for (i=1 ; i < argc ; i++) {
		printf("name = %s, type = ", argv[i]);

		if(lstat(argv[i], &file_info) < 0) {
			fprintf(stderr, "lstat error\n");
			continue;
		}

		if(S_ISREG(file_info.st_mode)) //regular 파일
			str = "regular";
		else if(S_ISDIR(file_info.st_mode)) // directory 파일
			str = "directory";
		else if(S_ISCHR(file_info.st_mode)) // character 파일
                        str = "character special";
		else if(S_ISBLK(file_info.st_mode)) // block 파일
                        str = "block special";
		else if(S_ISFIFO(file_info.st_mode)) // FIFO 파일
                        str = "FIFO";
		else if(S_ISLNK(file_info.st_mode)) // sybolic link
                        str = "symbolic link";
		else if(S_ISSOCK(file_info.st_mode)) //socket 파일
                        str = "socket";

		else 
                        str = "unknown mode"; //알려지지 않은 모드

	
	printf("%s\n",str);

	}

	
	gettimeofday(&end_t,NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);
}
