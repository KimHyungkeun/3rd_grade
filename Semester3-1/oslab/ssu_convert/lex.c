#include "ssu_convert.h"
#include "ssu_time.h"

int lookup(char ch) { // 연산자 처리함수
    switch (ch)
    {
        case '(':
            addChar();
            nextToken = LEFT_PAREN; // "(" 에 대한 토큰으로 지정
            break;

        case ')':
            addChar();
            nextToken = RIGHT_PAREN; // ")" 에 대한 토큰으로 지정
            break;

		case '{':
            addChar();
            nextToken = LEFT_BRACE; // "{" 에 대한 토큰으로 지정
            break;

        case '}':
            addChar();
            nextToken = RIGHT_BRACE; // "}" 에 대한 토큰으로 지정
            break;

		case '[':
            addChar();
            nextToken = LEFT_BRACKET; // "[" 에 대한 토큰으로 지정
            break;

        case ']':
            addChar();
            nextToken = RIGHT_BRACKET; // "]" 에 대한 토큰으로 지정
            break;

        case '+':
            addChar();
            nextToken = ADD_OP; // "+"에 대한 토큰으로 지정
            break;

        case '-' :
            addChar();
            nextToken = SUB_OP; // "-"에 대한 토큰으로 지정
            break;
        
        case '*' :
            addChar();
            nextToken = MULT_OP; // "*"에 대한 토큰으로 지정
            break;

        case '/' :
            addChar();
            nextToken = DIV_OP; // "/"에 대한 토큰으로 지정
            break;

		case '.' :
            addChar();
            nextToken = DOT_OP; // "."에 대한 토큰으로 지정
            break;

		case ',' :
            addChar();
            nextToken = SEMIDOT_OP; // ","에 대한 토큰으로 지정
            break;

		case ';' :
            addChar();
            nextToken = SEMICOLON; // ";"에 대한 토큰으로 지정
            break;
    
        default:
            addChar();
            nextToken = EOF; 
            break;
    }
    return nextToken; //지정된 토큰 반환

}

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar; //받아들인 문자 하나하나를 lexeme 배열에 넣는다.
        lexeme[lexLen] = 0; 
    }
    else
        printf("Error - lexeme is too long \n");
}

void getChar() {
    if((nextChar = getc(fp)) != EOF) { 
        if(isalpha(nextChar)) //받아온 char가 문자이면 
            charClass = LETTER; // LETTER 토큰으로 지정
        else if(isdigit(nextChar)) //받아온 char가 숫자라면
            charClass = DIGIT; // DIGIT 토큰으로 지정
        else 
            charClass = UNKNOWN; //미확인 토큰이라면 UNKNOWN으로 지정
    }
    else
        charClass = EOF; //어떤 경우도 아니라면 EOF로 지정
    
}

void getNonBlank() {
    while(isspace(nextChar)) //공백문자에 처리에 대한 함수
        getChar();
}

int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) //getChar에서 지정한 charclass값을 기준으로 LETTER, DIGIT, UNKNOWN을 판별
    {
        case LETTER :
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) { //charclass가 LETTER 또는 DIGIT이라면 
                addChar(); //문자를 lexeme 배열에 넣고
                getChar(); //또 다른 문자를 받아온다.
            }
            if(strcmp(lexeme,"for") == 0) {
                nextToken = FOR_CODE; //LETTER중에 for문에 대해서는 FOR_CODE를 적용
                break;
            }
            else if(strcmp(lexeme,"if") == 0) {
                nextToken = IF_CODE; //LETTER중에 if문에 대해서는 IF_CODE 적용
                break;
            }
            else if(strcmp(lexeme,"else") == 0) {
                nextToken = ELSE_CODE; //LETTER중에 else문에 대해서는 ELSE_CODE 적용
                break;
            }
            else if(strcmp(lexeme,"while") == 0) {
                nextToken = WHILE_CODE; //WHILE문에 대해서는 WHILE_CODE 적용
                break;
            }
            else if(strcmp(lexeme,"do") == 0) {
                nextToken = DO_CODE; //do에 대해서는 DO_CODE 적용
                break;
            }
            else if(strcmp(lexeme,"int") == 0) {
                nextToken = INT_CODE; //int에 대해선 INT_CODE 적용
                break;
            }
            else if(strcmp(lexeme,"float") == 0) {
                nextToken = FLOAT_CODE; //float에 대해서는 FLOAT_CODE 적용
                break;
            }
            else if(strcmp(lexeme,"switch") == 0) {
                nextToken = SWITCH_CODE; //switch에 대해서는 SWITCH_CODE 적용
                break;
            }
            else {
            nextToken = IDENT; //어떤 경우도 아니라면 identifier로 간주한다
            break;
            }

        case DIGIT :
            addChar();
            getChar();
            while (charClass == DIGIT) { //charclass가 DIGIT이면
                addChar(); //lexeme 배열에 문자들을 추가하고
                getChar(); //다음 문자를 받아온다.
            }
            nextToken = INT_LIT; //토큰은 INT_LIT으로 지정
            break;

        case UNKNOWN :
            lookup(nextChar); //미확인 문자에 대한 처리
            getChar();
            break;

        case EOF :
        nextToken = EOF; //EOF의 경우에는 lexeme배열에 "EOF"란 문자열을 집어넣는다.
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] =  0 ;
        break;
    }
    /*End of Switch syntax*/
    printf("Next token is : %d, Next lexeme is %s\n", nextToken, lexeme); //토큰 반환 결과를 보여준다.
    return nextToken;

}