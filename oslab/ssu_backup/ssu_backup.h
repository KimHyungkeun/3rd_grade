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

#define BUFFER_MAX 255

typedef struct backup_list {
    struct backup_list *prev;
    pthread_t tid;
    pid_t pid;
    char filepath[255];
    int period;
    char backup_opt[3];
    struct backup_list *next;
} Backup_list;


struct stat statbuf;
struct tm *tm_p;
time_t now;


FILE *log_fp;
Backup_list *head;
Backup_list *curr;

char command[BUFFER_MAX];
char filename[BUFFER_MAX];
char *prompt = "20142332>";
char backup_dir[BUFFER_MAX];
char logfile[BUFFER_MAX];
char regular_name[BUFFER_MAX];
char period[1];


int add_opt;
int add_opt_flag[4] = {0,0,0,0}; //옵션 플래그

void prompt_environment(void);
int add_command_analyzer(void);
void *add_function(void*);
void remove_function(void);
void list_function(void);
void compare_function(void);