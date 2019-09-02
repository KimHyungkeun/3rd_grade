#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>

#define BUFSIZE 2048
#define MAXLEN 64

#define HEADER 0
#define CODE 1

#define TRUE 2
#define FALSE 3

#define FORKFLAG "forkflag"
#define TEMPJAVA "tempjava"

#define MAXLINENUM 200

#define SECOND_TO_MICRO 1000000

#define MODE (O_RDWR|O_CREAT|O_TRUNC)

int tabFlag;
int forkFlag;

int optjFlag;
int optcFlag;
int optpFlag;
int optfFlag;
int optlFlag;
int optrFlag;

int openFlag = FALSE;
int writeFlag = FALSE;
int printfFlag = FALSE;
int scanfFlag = FALSE;
int closeFlag = FALSE;
int mainFlag = FALSE;
int publicFlag = FALSE;
int conFlag = FALSE;

void ssu_runtime(struct timeval* begin_t, struct timeval* end_t);

void do_r_Option(void);
void do_l_Option(void);
void do_f_Option(void);
void do_j_c_Option(char * fname);

void isOptrFlag(char isWrite);

void do_makeFile(char * fname);

char * makeClose(char * javaCode);
char * makeWrite(char * javaCode);
char * makeNewFile(char * javaCode);
char * makeDefine(char * javaCode);
char * makeArray(char * javaCode);
char * makeScanf(char * javaCode);
char * makePrintf(char * javaCode);

int getFileSize(char * fname);
int getLineNum(char * fname);
char * getFilename(char * javaCode);

void adjustMain(int mainPos);
int checkExistMain(char * fname);

void closeFile(void);

char* eliminateBrace(char *str, char ch);
void append(char * dst, char c);
char* leftTrim(char str[]);
char * toString(const char * src);
void removeEnd(char * buf);
char * replaceAll(char *s, const char *olds, const char *news);

void javaCmp(char * javaCode);
void insertToC(int fd, char * code, int position);

char * chkFuncformat(char * javaCode);
char * getConfunc(char * javaCode);
int  cmpConList(char * javaCode);


void findHeader(char * fune);
int checkHeader(char * code, char * header);

int openCFile(char * fname);
char * createCFile(char * javaCode);

char * checkValue(char * javaCode);
