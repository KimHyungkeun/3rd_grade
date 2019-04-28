#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFFER_SIZE 4096 //
#define HEADER_SIZE 2048 //

struct timeval begin_t, end_t;
struct stat statbuf;
char buffer[BUFFER_SIZE] = "\0";
char c_buffer[BUFFER_SIZE] = "\0";
char stackc_buffer[BUFFER_SIZE] = "\0";
char header_buffer[HEADER_SIZE] = "\0";
char header_new_buffer[HEADER_SIZE] = "\0";
char ch;


char filename[20]; //filename
char filename_backup[20];
char c_filename[20]; //new cfilename


int stackc_flag = 0;
int tab_flag = 0;
int bracket_delete_flag = 0;
int return_to_exit_flag = 0;

int header_flag[5] = {0,0,0,0,0};
int main_flag = 0;

FILE* fp;
FILE* newfp;
FILE* stackc_fp;
FILE* makefp;
FILE* headfp;

void convert_java_to_c(int* opt_flag); 
void option_java_to_c(int* opt_flag);
void create_makefile();
void find_header(int* opt_flag);

