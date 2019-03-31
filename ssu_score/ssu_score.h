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

#define BUFFER_SIZE 2048
#define WARNING 0.1
#define ERROR 0
#define LONG_PROCESS 0

struct ssu_score_table_format{
    char name[20];
    char comma;
    double score;
    char linejump;
} ;

struct total_score_table_format{
    char student[20];
    double score[49];
    double sum;
} ;

 DIR *dir__ans, *sub_ans, *dir_std, *sub_std; //dir_info : 처음 부모디렉터리, dir_* : DIR_* 내부의 디렉터리
   struct dirent  *entry_ans, *entry_sub_ans, *entry_std, *entry_sub_std; // dir_info_*를 readdir 하기 위한 변수, dir_*를 readdir 하기 위한 변수
   char dir_root_ans[30] = "ANS_DIR/"; //ANS_DIR  
   char dir_root_std[30] = "STD_DIR/"; //STD_DIR
   char dir_path_ans[30] = "\0"; //ANS_DIR  
   char dir_path_std[30] = "\0"; //STD_DIR
   char dir_path_backup_ans[30] = "\0"; // ANS_DIR/SUBDIR/로 돌아감
   char dir_path_backup_std[30] = "\0"; // STD_DIR/SUBDIR/로 돌아감
   char sub_path_ans_backup[30] = "\0";
   char sub_path_std_backup[30] = "\0";
   char child_filename[30] = "\0";
   char buf_ANS[BUFFER_SIZE]; // ANS 의 파일내용을 담기 위한 변수
   char buf_STD[BUFFER_SIZE]; // STD의 파일내용을 담기 위한 변수
   char buf_dirname[BUFFER_SIZE]; //bufarray for save directory_name
   int fd_ans, fd_std; //fd : 파일 디스크립터, count : 읽은 buf의 갯수
   int pro_count = 0;
   struct timeval begin_t, end_t; //시작시간과 끝나는 시간	
   struct ssu_score_table_format ssu_score_tab_for[49]; //ssu_score_table format
   struct total_score_table_format total_score_tab_for[20]; //total_score_table format
   time_t first, second;



void blank_problem_check(char *, char *);
void program_autocompile(char*, char*, int, int, char*, char*);
void program_problem_check(char *, char *);
void ssu_score_table_create(char *);
void score_table_create_proto(char *, char *);
void score_table_create(char *, char *, int);



