#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#define BUFFER_SIZE 4096 //버퍼 사이즈
#define WARNING 0.1 // WARNING 발생시 0.1점 감점
#define ERROR 0 //ERROR 발견시 0점 부여성
#define LONG_PROCESS 0 //프로그램 수행시간 5초 초과 시 0점 부여 

struct ssu_score_table_format{ //ssu_score.csv 표 형태
    char name[20]; //문제번호
    char comma; // "," csv 파일에서 콤마로 구분함
    double score; //score
    char linejump; // 개행문자
} ;

struct total_score_table_format{ //score.csv 포맷
    char student[20]; //학번
    double score[49]; //학생이 얻은 점수
    double sum; // 총점
} ;

 DIR *dir__ans, *sub_ans, *dir_std, *sub_std; //dir_info : 처음 부모디렉터리, dir_* : DIR_* 내부의 디렉터리
   struct dirent  *entry_ans, *entry_sub_ans, *entry_std, *entry_sub_std; // dir_info_*를 readdir 하기 위한 변수, dir_*를 readdir 하기 위한 변수
   char dir_root_ans[30] = "ANS_DIR/"; //ANS_DIR  
   char dir_root_std[30] = "STD_DIR/"; //STD_DIR
   char dir_path_ans[30] = "\0"; //ANS_DIR  
   char dir_path_std[30] = "\0"; //STD_DIR
   char dir_path_backup_ans[30] = "\0"; // ANS_DIR/로 돌아감
   char dir_path_backup_std[30] = "\0"; // STD_DIR/로 돌아감
   char sub_path_ans_backup[30] = "\0"; // ANS_DIR/SUBDIR/로 돌아감
   char sub_path_std_backup[30] = "\0"; // STD_DIR/SUBDIR/로 돌아감
   char buf_ANS[BUFFER_SIZE]; // ANS 의 파일내용을 담기 위한 변수
   char buf_STD[BUFFER_SIZE]; // STD의 파일내용을 담기 위한 변수
   char buf_ERR[BUFFER_SIZE]; // ERROR의 내용을 담기 위한 변수
   char buf_dirname[BUFFER_SIZE]; //디렉터리 이름을 담을 변수
   int fd_ans, fd_std; //fd : 파일 디스크립터, count : 읽은 buf의 갯수
   int pro_count = 0; //총 문제 수
   struct timeval begin_t, end_t; //시작시간과 끝나는 시간	
   struct ssu_score_table_format ssu_score_tab_for[49]; //ssu_score.csv 테이블에 대한 구조체
   struct total_score_table_format total_score_tab_for[20]; //score.csv에 대한 구조체
   time_t first, second; //파일 실행시간 측정



void blank_problem_check(char *, char *); //빈칸 채우기 문제 채점
void program_autocompile(char*, char*, int, int, char*, char*); //프로그래밍 문제 자동 컴파일
void program_problem_check(char *, char *); //프로그래밍 문제 채점
void ssu_score_table_create(char *); // ssu_score.csv 파일 생성하기
void score_table_create_proto(char *, char *); //채점하기 이전의 score.csv를 만든다.
void score_table_create(char *, char *, int); //채점 후의 최종 score.csv로 재생



