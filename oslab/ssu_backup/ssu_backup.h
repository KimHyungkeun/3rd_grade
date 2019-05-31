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

#define BUFFER_MAX 256

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
FILE *log_fp;

char command[BUFFER_MAX];
char command_tmp[BUFFER_MAX];
char filename[BUFFER_MAX];
char backup_dir[BUFFER_MAX];
char logfile[BUFFER_MAX];
char regular_name[BUFFER_MAX];
int recover_flag = 0;


void prompt_environment(void);
int add_command_analyzer(Backup_list*);
void *add_function(void*);
Backup_list* remove_function(Backup_list*);
void list_function(Backup_list*);
void compare_function(void);
void recover_function(Backup_list*, Backup_list*);