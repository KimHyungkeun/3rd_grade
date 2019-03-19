#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define WARNING 0.1
#define ERROR 0
#define LONG PROCESS 0

struct table_format {
    char name[10];
    char comma;
    int score;
    char line_jump;
};

char* filename[] = {"1-1.txt","1-2.txt","1-3.txt","1-4.txt","1-5.txt",
"2-1.txt","2-2.txt","2-3.txt","2-4.txt","3-1.txt","3-2.txt","3-3.txt","4-1.txt","4-2.txt","4-3.txt","4-4.txt",
"5-1.txt","5-2.txt","5-3.txt","5-4.txt","6-1.txt","6-2.txt","6-3.txt","6-4.txt","6-5.txt","7-1.txt","7-2.txt",
"7-3.txt","7-4.txt","7-5.txt","8-1.txt","8-2.txt","8-3.txt","8-4.txt","9-1.txt","9-2.txt","9-3.txt","9-4.txt",
"9-5.txt","9-6.txt","10-1.txt","10-2.txt","10-3.txt","10-4.txt","10-5.txt","11.c","12.c","13.c","14.c"};

void blank_problem_check(char *, char *);
void program_problem_check(char *, char *);
void score_table_create(char *);
char* txt_search(char *);
char* c_search(char *);
