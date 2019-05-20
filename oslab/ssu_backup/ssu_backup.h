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
    char filepath[255];
    int period;
    char backup_opt[3];
} Backup_list;

struct stat statbuf;
struct tm *tm_p;
time_t now;

char command[BUFFER_MAX];
char *prompt = "20142332>";
char directory_name[BUFFER_MAX];
char regular_name[BUFFER_MAX];

void prompt_environment();
void add_function();
void remove_function();