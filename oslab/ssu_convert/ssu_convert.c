#include "ssu_convert.h"
#include "ssu_time.h"

int main(int argc, char* argv[]) {

   int opt;
   int opt_flag[5] = {0,0,0,0,0};

	gettimeofday(&begin_t, NULL);

	if(argc < 2) {
		fprintf(stderr, "Usage : %s javafile [option]\n", argv[0]);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	strcpy(filename,argv[1]);

	while ((opt = getopt(argc, argv, "jcflr")) != -1) { //옵션은 j,c,f,l,r 5종류가 있다
    switch(opt) {

        case 'j' : // 옵션
            opt_flag[0] = 1;
            break;
         
        case 'c' : // 옵션
            opt_flag[1] = 1;
            break;
        
        case 'f' : // 옵션
            opt_flag[2] = 1;
            break;

        case 'l' : // 옵션
            opt_flag[3] = 1;
            break;
		
		case 'r' : // 옵션
            opt_flag[4] = 1;
            break;
		
		case '?':
			gettimeofday(&end_t,NULL);
    		ssu_runtime(&begin_t, &end_t);
			exit(1);

    }
   }

	convert_java_to_c(opt_flag);
    option_java_to_c(opt_flag);
    create_makefile();
	
	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void convert_java_to_c(int* opt_flag) {

    char *filename_ptr;
    char *ptr;
    char *tmp_ptr;
    char *sub_ptr;
    char tmp_str[100];

    int ptr_count = 0;
    int sub_ptr_count = 0;
    int chr=0;
    
    
    //strcpy(filename, "q2.java");
    if((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "open error\n");
		exit(1);
	}

    strcpy(filename_backup, filename);
    
    filename_ptr = strtok(filename,".");
    sprintf(c_filename,"%s%s",filename_ptr,".c");
    if((newfp = fopen(c_filename, "w+")) == NULL) {
        	fprintf(stderr, "creat error\n");
            exit(1);
    }
    
    fread(buffer, BUFFER_SIZE,1,fp);
	fseek(fp, 0 , SEEK_SET);

  
    while(buffer[chr] != '\n') {
        chr++;
    }
    
   
    //chr = 0;

    //buffer[statbuf.st_size - 1] = ' ';
    ptr = strtok(buffer, "\n");
    while (ptr != NULL)              
    {
        strcpy(tmp_str, ptr);
        //printf("\\n count : %d\n", chr);
        //printf("TOKEN : %s\n", tmp_str);

        if(strcmp(ptr,"}") == 0) {
            strcpy(tmp_str, "\0");
        }

        //printf("%d\n", strcmp(tmp_str, "import java.util.Scanner;"));
        
        if(strstr(tmp_str, "import java.util.Scanner;") != NULL) {
            strcpy(tmp_str, "#include <stdio.h>\n#include <stdlib.h>\n");
        }

        else if(strstr(tmp_str, "import java.io.File;") != NULL) {
            strcpy(tmp_str, "#include <stdio.h>\n#include <stdlib.h>\n#include <unistd.h>\n#include <fcntl.h>");
        }

        else if(strstr(tmp_str, "import") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if(strstr(tmp_str, "int top;") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if(strstr(tmp_str, "int[] stack;") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if(strstr(tmp_str, "public Stack") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if(strstr(tmp_str, "top = -1;") != NULL) {
            strcpy(tmp_str, "int top = -1;");
        }

        else if(strstr(tmp_str, "stack = new int[STACK_SIZE];") != NULL) {
            strcpy(tmp_str, "int stack[STACK_SIZE];");
            
        }

        else if(strstr(tmp_str, "File file = new File(\"q3java.txt\");") != NULL) {
            strcpy(tmp_str, "\t\tFILE* file = fopen(\"q3java.txt\", \"w+\");");
        }

        else if(strstr(tmp_str, "FileWriter writer = new FileWriter(file, false);") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if(strstr(tmp_str, "writer.write(\"2019 OSLAB\\n\");") != NULL) {
            //printf("Check\n");
            strcpy(tmp_str, "\t\tfprintf(file, \"%s\" ,\"2019 OSLAB\\n\");");
        }

        else if(strstr(tmp_str, "writer.write(\"Linux System Programming\\n\");") != NULL) {
             //printf("Check\n");
            strcpy(tmp_str, "\t\tfprintf(file, \"%s\" ,\"Linux System Programming\\n\");");
        }
        
        else if(strstr(tmp_str, "writer.flush();") != NULL) {
            strcpy(tmp_str, "\t\tfflush(stdin);");
        }

        else if(strstr(tmp_str, "if(writer != null)") != NULL) {
            strcpy(tmp_str, "\0");
        }
        
        else if(strstr(tmp_str, "writer.close") != NULL) {
            strcpy(tmp_str, "\t\tfclose(file);");
        }

        else if(strstr(tmp_str, "public static final int STACK_SIZE = 10;") != NULL) {
            strcpy(tmp_str, "#define STACK_SIZE 10\n\nint peek();\nvoid push(int);\nint pop();\nvoid printStack();\n");
        }

        else if(strstr(tmp_str, "Stack st = new Stack();") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if(strstr(tmp_str, "class Stack") != NULL) {
            strcpy(tmp_str, "#include <stdio.h>\n#include <stdlib.h>\n#include <unistd.h>\n#include <fcntl.h>");
            stackc_flag = 1;
        }

        else if(strstr(tmp_str, "public class") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if((sub_ptr = strstr(tmp_str, "public void")) != NULL) {
            strcpy(tmp_str, sub_ptr + 6);
        }

        else if((sub_ptr = strstr(tmp_str, "public int")) != NULL) {
            strcpy(tmp_str, sub_ptr + 6);
        }

        else if(strstr(tmp_str, "public static void main(String args[])") != NULL) {
            strcpy(tmp_str, "int main(void){");
        }

        else if(strstr(tmp_str, "public static void main(String[] args)") != NULL) {
            strcpy(tmp_str, "int main(void){");
        }

        else if(strstr(tmp_str, "Scanner scn = new Scanner(System.in)") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if((sub_ptr = strstr(tmp_str, "System.out.printf")) != NULL) {
             strcpy(tmp_str, sub_ptr + 11);
             if(strstr(tmp_str, "st.peek") != NULL) {
                strcpy(tmp_str, "printf(\"TOP OF STACK : %d\\n\", peek());");
             }
        }

        else if((sub_ptr = strstr(tmp_str, "st.")) != NULL) {
            strcpy(tmp_str, sub_ptr+3);
        }

        else if(strstr(tmp_str, "num = scn.nextInt();") != NULL) {
            strcpy(tmp_str, "\t\tscanf(\"%d\",&num);");
        }

        else if(strstr(tmp_str, "return ;") != NULL) {
            strcpy(tmp_str, "\t\texit(0);");
        }


        fprintf(newfp, "%s\n", tmp_str);

        if(opt_flag[4] == 1) {
        printf("%s\n",tmp_str);
        sleep(1);
        }
        //printf("ptr_count : %d\n", ptr_count);
        ptr_count ++;
        ptr = strtok(NULL, "\n");    
    }
 
    fclose(newfp);
    fclose(fp);


    if(stackc_flag == 1) {

        //printf("%s\n",c_filename);
        if((stackc_fp = fopen(c_filename, "r")) == NULL) {
        	fprintf(stderr, "creat error for %s\n", c_filename);
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }

        fread(c_buffer, BUFFER_SIZE, 1 ,stackc_fp);
        //printf("%s\n", c_buffer);
	    fseek(stackc_fp, 0 , SEEK_SET);
        fclose(stackc_fp);

        if((stackc_fp = fopen("Stack.c", "w+")) == NULL) {
        	fprintf(stderr, "creat error for Stack.c\n");
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }
        
        ptr = strtok(c_buffer, "\n");
        while(ptr != NULL){
            if(strstr(ptr,"int main(void){") != NULL)
                break;
            fprintf(stackc_fp,"%s\n", ptr);
            ptr = strtok(NULL,"\n");
        }

        fclose(stackc_fp);
    }

    //printf("CheckPoint\n");
    printf("%s converting is finished!\n",c_filename);

}

void option_java_to_c(int* opt_flag) {

	int length;
	int line_count = 0;
	/**/
	strcpy(filename, filename_backup);

	if((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "open error for %s\n",filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

    if((newfp = fopen(c_filename, "r")) == NULL) {
		fprintf(stderr, "open error for %s\n",c_filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	fread(buffer, BUFFER_SIZE,1,fp);
    fread(c_buffer, BUFFER_SIZE,1,newfp);

	fseek(fp, 0 , SEEK_SET);
    fseek(newfp, 0 , SEEK_SET);
	
	if(opt_flag[0] == 1) { //option j
		printf("%s", buffer);
	}

    if(opt_flag[1] == 1) { //option c

        printf("%s", c_buffer);

         if(stackc_flag == 1) {

            memset(c_buffer, 0, BUFFER_SIZE);

            if((stackc_fp = fopen("Stack.c","r")) == NULL){
            fprintf(stderr, "No Stack.c file\n");
            }

            else {
            fread(c_buffer, BUFFER_SIZE, 1, stackc_fp);
            printf("%s", c_buffer);
            fclose(stackc_fp);
            }
        }
	}

	if(opt_flag[2] == 1) { //option f
		stat(filename, &statbuf);
		printf("%s file size is %ld bytes.\n", filename, statbuf.st_size);
        stat(c_filename, &statbuf);
        printf("%s file size is %ld bytes.\n", c_filename, statbuf.st_size);
        
        if(stackc_flag == 1) {

            if((stackc_fp = fopen("Stack.c","r")) == NULL){
            fprintf(stderr, "No Stack.c file\n");
            }

            else {
            stat("Stack.c", &statbuf);
            printf("%s file size is %ld bytes.\n", "Stack.c", statbuf.st_size);
            fclose(stackc_fp);
            }
        }
	}

	if(opt_flag[3] == 1) { //option l
		
		while (!feof(fp)){//파일의 끝을 만나지 않았다면 반복
        	ch = fgetc(fp);//파일에서 하나의 문자를 읽음
        		if (ch == '\n')//개행 문자일 때
        		{
            	line_count++;//라인 번호 1 증가
        		}
    		}
		
		printf("%s line is %d lines\n", filename, line_count);

        line_count = 0;

        while (!feof(newfp)){//파일의 끝을 만나지 않았다면 반복
        	ch = fgetc(newfp);//파일에서 하나의 문자를 읽음
        		if (ch == '\n')//개행 문자일 때
        		{
            	line_count++;//라인 번호 1 증가
        		}
    		}
		
		printf("%s line is %d lines\n", c_filename, line_count);

        line_count = 0;
        if(stackc_flag == 1) {

            if((stackc_fp = fopen("Stack.c","r")) == NULL){
            fprintf(stderr, "No Stack.c file\n");
            }

            else {
                while (!feof(stackc_fp)){//파일의 끝을 만나지 않았다면 반복
        	    ch = fgetc(stackc_fp);//파일에서 하나의 문자를 읽음
        		    if (ch == '\n')//개행 문자일 때
        		    {
            	    line_count++;//라인 번호 1 증가
        		    }
    		    }
            }

            printf("%s line is %d lines\n", "Stack.c", line_count);
            fclose(stackc_fp);
        }
		
	}

	fseek(fp, 0 , SEEK_SET);
    fseek(newfp, 0 , SEEK_SET);
	
	/*getChar(); //전채 식에서 문자들을 하나씩 받는다 
        do {
            lex(); //각 식들의 요소 하나하나를 토큰화하여 판별한다.  
        }while( nextToken != EOF);*/

	fclose(fp);
    fclose(newfp);
	
}



void create_makefile() {

    if((makefp = fopen("Makefile", "w+")) == NULL) {
		fprintf(stderr, "open error\n");
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

    fprintf(makefp,"%s","q1 :\n\tgcc q1.c -o q1\n");
    fprintf(makefp,"%s","Stack :\n\tgcc Stack.c -o Stack\n");
    fprintf(makefp,"%s","q2 :\n\tgcc q2.c -o q2\n");
    fprintf(makefp,"%s","q3 :\n\tgcc q3.c -o q3\n");

    fclose(makefp);
}

/*
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

		case '=':
            addChar();
            nextToken = ASSIGN_OP; // "=" 에 대한 토큰으로 지정
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

		case '\n' :
            addChar();
            nextToken = LINE_CHAR; // ";"에 대한 토큰으로 지정
            break;
    
        default:
            addChar();
            nextToken = EOF; 
            break;
    }
    return nextToken; //지정된 토큰 반환

}

void addChar() {
 
		
        lexeme[lexLen++] = nextChar; //받아들인 문자 하나하나를 lexeme 배열에 넣는다.
		//printf("%c\n",lexeme[lexLen]);
        lexeme[lexLen] = 0; 
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
*/

/*
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
    //printf("Next token is : %d, Next lexeme is %s\n", nextToken, lexeme); //토큰 반환 결과를 보여준다.
    //return nextToken;
//}


