#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "ssu_time.h"

#define DIRECTORY_SIZE MAXNAMLEN

int main(int argc, char* argv[])
{

	struct dirent *dentry;
	struct timeval begin_t, end_t;
	struct stat statbuf;
	char filename[DIRECTORY_SIZE+1];
	DIR *dirp;

	gettimeofday(&begin_t, NULL);

	if(argc < 2) {
	fprintf(stderr,"usage : %s <directory>\n",argv[0]);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(1);
	}

	if((dirp = opendir(argv[1])) == NULL || chdir(argv[1]) == -1) {
	fprintf(stderr,"opendir, chdir error for %s\n",argv[1]); //디렉터리를 열거나 현 작업디렉터리 변경에 실패한 경우 에러출력
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(1);
	}

	while((dentry = readdir(dirp)) != NULL) {

		if(dentry -> d_ino == 0) //해당 디렉터리의 inode가 0이면 다음디렉터리에 대해 읽는다
			continue;

		memcpy(filename, dentry -> d_name, DIRECTORY_SIZE); //DIRECTORY_SIZE만큼 메모리를 카피

		if(stat(filename, &statbuf) == -1) {
		fprintf(stderr, "stat error for %s\n",filename);
		break;
		}

		if((statbuf.st_mode & S_IFMT) == S_IFREG)
			printf("%-14s %ld\n",filename,statbuf.st_size); //해당파일의 크기 출력
		else
			printf("%-14s\n",filename);
	}

	
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	exit(0);
}

