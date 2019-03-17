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
#define SECOND_TO_MICRO 1000000

void blank_problem_check(char *, char *);
void program_problem_check(char *, char *);
char* txt_search(char *);
char* c_search(char *);
