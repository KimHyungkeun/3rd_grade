#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFFER_SIZE 4096 //버퍼 사이즈
#define HEADER_SIZE 2048 //헤더가 들어갈 사이즈

struct timeval begin_t, end_t;
struct stat statbuf;

char buffer[BUFFER_SIZE] = "\0"; //java내용을 담을 버퍼
char c_buffer[BUFFER_SIZE] = "\0"; //c내용을 담을 버퍼
char stackc_buffer[BUFFER_SIZE] = "\0"; // stackc내용을 담을 버퍼
char header_buffer[HEADER_SIZE] = "\0"; //헤더테이블을 담을 버퍼
char header_new_buffer[HEADER_SIZE] = "\0"; //필요한 헤더들만 담을 버퍼
char ch;


char filename[20]; //filename
char filename_backup[20]; //파일이름 포맷변경을 위한 백업용 배열
char c_filename[20]; //new cfilename


int stackc_flag = 0; //stackc 생성시의 작동할 플래그
int tab_flag = 0; //해당 문장에 탭이 있을 경우 작동
int bracket_delete_flag = 0; //중괄호를 지우기 위한 플래그
int return_to_exit_flag = 0; //return을 exit로 바꾸기 위한 플래그

int header_flag[5] = {0,0,0,0,0}; //헤더테이블 각각의 요소에 대한 플래그
int main_flag = 0; //메인함수를 만났을 시 작동하는 플래그

FILE* fp; //자바 파일 포인터
FILE* newfp; //c파일 포인터
FILE* stackc_fp; //stackc 파일 포인터
FILE* makefp; //make파일 포인터
FILE* headfp; //헤더파일 포인터

void convert_java_to_c(int* opt_flag); //자바를 씨로 변경해주는 함수
void createfile_java_to_c(int* opt_flag); //새 파일을 생성. (변경된 내용을 담을 새 파일)
void create_makefile(); //make파일을 만드는 함수
void option_java_to_c(int* opt_flag); //옵션을 처리하는 함수
void find_header(int* opt_flag); //필요한 헤더들을 테이블에서 가져와서 넣어주는 함수

