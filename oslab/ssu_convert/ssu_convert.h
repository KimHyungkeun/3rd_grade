#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFFER_SIZE 4096

struct timeval begin_t, end_t;
struct stat statbuf;
char buffer[BUFFER_SIZE];
char c_buffer[BUFFER_SIZE];

int charClass;
char ch;
char lexeme[BUFFER_SIZE];

char filename[20]; //filename
char filename_backup[20];
char c_filename[20]; //new cfilename
char nextChar;
int lexLen;
int token;
int nextToken;
int stackc_flag = 0;

FILE* fp;
FILE* newfp;
FILE* stackc_fp;
FILE* makefp;

#define LETTER 0 //문자열에 속하는지 대한 확인
#define DIGIT 1 //숫자에 속하는지애 대한 확인
#define UNKNOWN 99 //미확인 코드

#define INT_LIT 10 // 숫자에 대한 코드
#define IDENT 11 // identifier 코드

#define QUOT 14 // '
#define DOUBLE_QUOT 15 // ""
#define LINE_CHAR 16 // "\n"
#define SEMICOLON 17 // ";"
#define SEMIDOT_OP 18 // ","
#define DOT_OP 19 // "."
#define ASSIGN_OP 20 // "=" 코드
#define ADD_OP 21 // "+" 코드
#define SUB_OP 22 // "-" 코드
#define MULT_OP 23 // "*" 코드
#define DIV_OP 24 // "/" 코드
#define LEFT_PAREN 25 // "(" 코드
#define RIGHT_PAREN 26 // ")" 코드
#define LEFT_BRACE 27 // "{"
#define RIGHT_BRACE 28 // "}"
#define LEFT_BRACKET 29 // "["
#define RIGHT_BRACKET 30 // "]"

#define FOR_CODE 31 // "for" 코드
#define IF_CODE 32 // "if" 코드
#define ELSE_CODE 33 // "else" 코드
#define WHILE_CODE 34 // "while" 코드
#define DO_CODE 35 // "do" 코드
#define INT_CODE 36 // "int" 코드
#define FLOAT_CODE 37 // "float" 코드
#define SWITCH_CODE 38 // "switch" 코드

void addChar(); //문자를 추가하는 함수
void getChar(); //식으로부터 문자를 받아들여오는 함수
void getNonBlank(); //공백문자 처리 함수
int lex(); //식을 토큰별로 나누는 함수

void convert_java_to_c();
void option_java_to_c(int* opt_flag);
void create_makefile();

