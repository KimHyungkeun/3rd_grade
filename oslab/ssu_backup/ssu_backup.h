#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFFER_MAX 256 //버퍼 최대크기

typedef struct backup_list {
    struct backup_list *prev; //이전노드 가리킴
    pthread_t tid; //쓰레드 아이디
    pid_t pid; //프로세스 아이디
    char filepath[255]; //파일 경로
    int period; //PERIOD
    char backup_opt[3]; //옵션 
    struct backup_list *next; //다음노드 가리킴
} Backup_list; //백업리스트 구조체


struct stat statbuf; //입력한 파일의 크기를 알아보기 위한 변수
FILE *log_fp; //로그파일 파일포인터

char command[BUFFER_MAX]; //프롬프트에 입력하게 될 커맨드
char command_tmp[BUFFER_MAX]; //입력한 커맨드 내용을 임시로 담아둔다
char filename[BUFFER_MAX]; //프롬프트에 입력하게 될 파일이름
char backup_dir[BUFFER_MAX]; //생성될 백업디렉토리 이름
char logfile[BUFFER_MAX]; //생성될 로그파일 이름
char regular_name[BUFFER_MAX]; //일반파일임을 구별하기 위한 배열
int recover_flag = 0; //recover기능에 따른 플래그 활성화


void prompt_environment(void); //프롬프트 환경 함수
int add_command_analyzer(Backup_list*); //add 명렁어 구문 분석
void *add_function(void*); //add 함수
Backup_list* remove_function(Backup_list*); //remove 함수
void list_function(Backup_list*); //list 함수
void compare_function(void); //compare 함수
void recover_function(Backup_list*, Backup_list*); //recover 함수