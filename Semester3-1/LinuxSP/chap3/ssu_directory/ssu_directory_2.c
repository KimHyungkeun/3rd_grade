#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include "ssu_time.h"

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define MAX_PATH_GUESSED 1024

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

char *pathname; //파일 이름
char command[LINE_MAX], grep_cmd[LINE_MAX];

int ssu_do_grep(void) {
    struct dirent *dirp; //디렉터리 entry에 필요한 변수
    struct stat statbuf;
    char *ptr;
    DIR* dp;

    if(lstat(pathname, &statbuf) < 0) {
        fprintf(stderr, "lstat error for %s\n", pathname); //lstat error 시 에러
        return 0;
    }

    if(S_ISDIR(statbuf.st_mode) == 0) {
        sprintf(command, "%s %s", grep_cmd, pathname);
        printf("%s : \n", pathname);
        system(command);
        return 0;
    }

    ptr = pathname + strlen(pathname);
    *ptr++ = '/';
    *ptr = '\0';

    if((dp = opendir(pathname)) == NULL) {
        fprintf(stderr, "opendir error for %s\n", pathname); //디렉터리 오픈 실패시 에러 출력
        return 0;
    }

    while((dirp = readdir(dp)) != NULL) //디렉터리를 읽어들인다
        if(strcmp(dirp -> d_name, ".") && strcmp(dirp -> d_name,"..")) {
            strcpy(ptr, dirp -> d_name); //.이나 ..은 생략한다

            if(ssu_do_grep() < 0)
                break;
        }

    ptr[-1] = 0; //해당 위치에서 한칸 전으로 이동시킨다.
    closedir(dp);
    return 0;
}

void ssu_make_grep(int argc, char* argv[]) {
    int i;
    strcpy(grep_cmd, " grep");

    for(i=1;i<argc-1;i++) {
        strcat(grep_cmd, " ");
        strcat(grep_cmd, argv[i]);
    }
}

int main(int argc, char *argv[]) {

    struct timeval begin_t, end_t;
	gettimeofday(&begin_t, NULL);

    if (argc < 2) {
        fprintf(stderr, "usage : %s <-CVbchilnsvwx> <-num> <-A num> <-B num> <-f file> \n" 
                                        "                  <-e> expr <directory>\n", argv[0]);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
        exit(1);
    }

    if(pathmax == 0) {
        if((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
            pathmax = MAX_PATH_GUESSED;
        else
            pathmax++;
    }

    if((pathname = (char*) malloc(pathmax+1)) == NULL) { //동적할당 
        fprintf(stderr, "malloc error\n");
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
        exit(1);
    }

    strcpy(pathname, argv[argc-1]);
    ssu_make_grep(argc, argv);
    ssu_do_grep();
    gettimeofday(&end_t, NULL); 
    ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
    exit(0);

}
