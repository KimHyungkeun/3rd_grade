#include "ssu_convert.h"
#include "ssu_time.h"

int main(int argc, char* argv[]) {

   int opt;
   int opt_flag[5] = {0,0,0,0,0}; //옵션 플래그

	gettimeofday(&begin_t, NULL);

	if(argc < 2) {
		fprintf(stderr, "Usage : %s javafile [option]\n", argv[0]); //비정상적인 명령어 입력시 에러 출력
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	strcpy(filename,argv[1]); //바꾸려는 자바파일의 이름을 넣음

	while ((opt = getopt(argc, argv, "jcflp")) != -1) { //옵션은 j,c,f,l,p 5종류가 있다
    switch(opt) {

        case 'j' : // 옵션
            opt_flag[0] = 1; //자바파일 보이기
            break;
         
        case 'c' : // 옵션
            opt_flag[1] = 1; //변경된 c파일 보이기
            break;
        
        case 'f' : // 옵션
            opt_flag[2] = 1; //파일 크기 출력
            break;

        case 'l' : // 옵션
            opt_flag[3] = 1; //라인수 출력
            break;
		
		case 'p' : // 옵션
            opt_flag[4] = 1; //변경된 함수들을 보여줌
            break;
		
		case '?': //잘못된 옵션 입력시 종료
			gettimeofday(&end_t,NULL);
    		ssu_runtime(&begin_t, &end_t);
			exit(1);

    }
   }

	convert_java_to_c(opt_flag); //자바를 c로 바꾸기
    createfile_java_to_c(opt_flag); //옵션 처리
    find_header(opt_flag); //헤더파일 찾기
    option_java_to_c(opt_flag);
    create_makefile(); //makefile 만들기
    
	
	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void convert_java_to_c(int* opt_flag) { //자바를 c로 바꾸는 함수

    char *filename_ptr; //파일이름(.앞에 붙는 이름)
    char *ptr;
    char *tmp_ptr;
    char *sub_ptr;
    char tmp_str[150];
    char tabadd_str[100];
    char c_buffer_backup[BUFFER_SIZE];

    int ptr_count = 0;
    int sub_ptr_count = 0;
    int chr=0;
    int idx=0;
    int print_count = 0;
    int c_buffer_length;
    int total_length = 0;
    
    
    
    if((fp = fopen(filename, "r")) == NULL) { //자바파일을 읽어들인다
		fprintf(stderr, "open error for %s\n", filename);
		exit(1);
	}

    strcpy(filename_backup, filename); //확장자를 제외한 파일명만 추출하기위해 백업용 배열에 넣어둔다
    
    filename_ptr = strtok(filename,".");
    sprintf(c_filename,"%s%s",filename_ptr,".c"); // 만들어낼 c파일 이름을 지정한다
    if((newfp = fopen(c_filename, "w+")) == NULL) { //해당 파일을 열어둔다
        	fprintf(stderr, "open error for %s\n", c_filename);
            exit(1);
    }
    
    fread(buffer, BUFFER_SIZE,1,fp); //버퍼에 자바파일 내용을 담아둔다
	fseek(fp, 0 , SEEK_SET);
  
  
    while(buffer[chr] != '\n') {
        chr++; //문자의 수들을 모두 세어본다. 단, 개행문자는 제외
    }
    
   
    
     printf("%s converting is finished!\n",c_filename);


    ptr = strtok(buffer, "\n"); //개행문자 기준으로 문자열을 하나씩 읽어들인다
    while (ptr != NULL)              
    {
        strcpy(tmp_str, ptr); //임시 배열에 현 포인터가 가리키는 문자열을 저장
        

        if(strcmp(ptr,"}") == 0) {
            strcpy(tmp_str, "\0");  //닫는 중괄호가 발견되면 제거
        }

        if(bracket_delete_flag == 1) {
            strcpy(tmp_str, "\0"); //괄호 표시가 있을 경우 해당 문자를 제거
            bracket_delete_flag = 0;
        }

        
        if(strstr(tmp_str, "import java.util.Scanner;") != NULL) {
           
            strcpy(tmp_str, "\0"); //위 import문을 발견하면 제거한다
        }

        else if(strstr(tmp_str, "import java.io.File;") != NULL) {
            
            strcpy(tmp_str, "\0"); // 위 import문을 발견하면 제거한다
        }

        else if(strstr(tmp_str, "import") != NULL) {
            strcpy(tmp_str, "\0"); //위 import문을 발견하면 제거한다
        }

        else if(strstr(tmp_str, "int top;") != NULL) {
            strcpy(tmp_str, "\0"); // int top을 발견하면 제거한다
        }

        else if(strstr(tmp_str, "int[] stack;") != NULL) {
            strcpy(tmp_str, "\0"); // int[] stack; 을 발견하면 제거한다
        }

        else if(strstr(tmp_str, "public Stack") != NULL) {
            strcpy(tmp_str, "\0"); // public Stack을 발견하면 제거한다
        }

        else if(strstr(tmp_str, "top = -1;") != NULL) {
            strcpy(tmp_str, "int top = -1;"); // top = -1을 발견하면 선언과 동시에 정의한다 
        }

        else if(strstr(tmp_str, "stack = new int[STACK_SIZE];") != NULL) {
            strcpy(tmp_str, "int stack[STACK_SIZE];"); // 위와같은 배열 형태일 경우 c에 맞게 배정해준다.
            bracket_delete_flag = 1;
        }

        else if(strstr(tmp_str, "File file = new File") != NULL) {
            strcpy(tmp_str, "\t\tFILE* file;\n\t\tif((file = fopen(\"q3java.txt\", \"w+\")) == NULL){\n\t\t\tfprintf(stderr,\"open error\\n\");\n\t\t\texit(1);\n\t\t}");
            if(opt_flag[4] == 1) { // p옵션 설정시 위 형태가 c파일에서 어떻게 변형되는지 보여준다. 
                printf("File file -> fopen\n");
            }
        }

        else if(strstr(tmp_str, "FileWriter writer = new FileWriter(file, false);") != NULL) {
            strcpy(tmp_str, "\0"); //위 writer 형태를 발견할 경우 해당문장을 제거한다
            if(opt_flag[4] == 1) {
                printf("FileWriter -> fprintf\n"); // p옵션 설정시 위 형태가 c파일에서 어떻게 변형되는지 보여준다. 
            }
        }

        else if(strstr(tmp_str, "writer.write(\"2019 OSLAB\\n\");") != NULL) {
            
            strcpy(tmp_str, "\t\tfprintf(file, \"%s\" ,\"2019 OSLAB\\n\");"); //위와같은 형태를 발견하면 c파일에서 어떻게 변형되는지 보여준다. 
        }

        else if(strstr(tmp_str, "writer.write(\"Linux System Programming\\n\");") != NULL) {
             
            strcpy(tmp_str, "\t\tfprintf(file, \"%s\" ,\"Linux System Programming\\n\");"); //위와같은 형태를 발견하면 c파일에서 어떻게 변형되는지 보여준다. 
        }
        
        else if(strstr(tmp_str, "writer.flush();") != NULL) {
            strcpy(tmp_str, "\t\tfflush(stdin);"); //위와같은 형태를 발견하면 c파일에서 어떻게 변형되는지 보여준다. 
            if(opt_flag[4] == 1) {
                printf("writer.flush() -> fflush(stdin)\n"); // p옵션 설정시 위 형태가 c파일에서 어떻게 변형되는지 보여준다.
            }
        }

        else if(strstr(tmp_str, "if(writer != null)") != NULL) {
            strcpy(tmp_str, "\0"); //위 형태를 발견했을 경우 제거한다.
        }
        
        else if(strstr(tmp_str, "writer.close") != NULL) {
            strcpy(tmp_str, "\t\tfclose(file);"); //위 형태를 발견했을 경우 fclose(file);로 변경해준다
        }

        else if(strstr(tmp_str, "public static final int STACK_SIZE = 10;") != NULL) {
            strcpy(tmp_str, "#define STACK_SIZE 10\n\nint peek();\nvoid push(int);\nint pop();\nvoid printStack();\n");
            //매크로함수를 설정하고 동시에 함수들까지 미리 선언해준다.
        }

        else if(strstr(tmp_str, "Stack st = new Stack();") != NULL) {
            strcpy(tmp_str, "\0"); //위 형태를 발견했을 경우 제거한다.
        }

        else if(strstr(tmp_str, "class Stack") != NULL) {
            
            strcpy(tmp_str, "\0"); //위 형태를 발견했을 경우 제거한다.
            stackc_flag = 1; // stack.c 파일 오픈 플래그가 활성화된다.
        }

        else if(strstr(tmp_str, "public class") != NULL) {
            strcpy(tmp_str, "\0"); //위 형태를 발견했을 경우 제거한다.
        }

        else if((sub_ptr = strstr(tmp_str, "public void")) != NULL) {
            strcpy(tmp_str, sub_ptr + 6); //위 형태를 발견했을 경우 public이란 글자는 제외한다
           
        }

        else if((sub_ptr = strstr(tmp_str, "public int")) != NULL) {
            strcpy(tmp_str, sub_ptr + 6); //위 형태를 발견했을 경우 public이란 글자는 제외한다
        
        }

        else if(strstr(tmp_str, "public static void main") != NULL) {
            strcpy(tmp_str, "int main(void){"); //메인함수일때는 int main(void)로 변경한다.
            return_to_exit_flag = 1;
            if(opt_flag[4] == 1) {
                printf("public static void main -> int main(void)\n"); // p옵션 설정시 위 형태가 c파일에서 어떻게 변형되는지 보여준다. 
            }
        }

        else if(strstr(tmp_str, "Scanner scn = new Scanner(System.in)") != NULL) {
            strcpy(tmp_str, "\0"); //위 형태를 발견했을 경우 제거한다.
        }

        else if((sub_ptr = strstr(tmp_str, "System.out.printf")) != NULL) {
             tab_flag = 1; //탭을 활성화 시킨다
             strcpy(tmp_str, sub_ptr + 11); //위와같은 형태를 발견하면 printf 부분만 추출한다

            if(opt_flag[4] == 1 && print_count < 1) {
                printf("System.out.printf -> printf\n"); // p옵션 설정시 위 형태가 c파일에서 어떻게 변형되는지 보여준다. 
                print_count++;
            }

             if(strstr(tmp_str, "st.peek") != NULL) {
                strcpy(tmp_str, "printf(\"TOP OF STACK : %d\\n\", peek());");
                tab_flag = 0; //탭을 비활성화 시킨다
             }

             
        }

        else if((sub_ptr = strstr(tmp_str, "st.")) != NULL) {
            strcpy(tmp_str, sub_ptr+3); //st. 란 글자를 제외하고 쓴다
        }

        else if(strstr(tmp_str, "scn.nextInt();") != NULL) {
            strcpy(tmp_str, "\t\tscanf(\"%d\",&num);");
            if(opt_flag[4] == 1) {
                printf("scn.nextInt -> scanf\n"); // scn.nextInt()를 scanf로 바꾼다.
            }
        }

        else if(strstr(tmp_str, "return") != NULL) {
            if(return_to_exit_flag == 1) {
            strcpy(tmp_str, "\t\texit(0);"); //return을 exit로 바꾼다/
            return_to_exit_flag = 0; //관련 플래그 비활성화
            }
        }


        if(tab_flag == 1) {
        fprintf(newfp, "\t\t%s\n", tmp_str); //탭플래그 활성화 시 문장앞에 탭을 2번 넣는다
        tab_flag = 0;
        }

        else {
        fprintf(newfp, "%s\n", tmp_str); //기본적으로 해당 문자를 c파일에 하나씩 집어넣는다
        }

        
        ptr_count ++; //몇번째 줄인지 세기 위한 변수
        ptr = strtok(NULL, "\n"); //다음 문자열로 넘어간다
    }

    fclose(newfp);
    fclose(fp);

    if(stackc_flag == 1) { //만약 stack.c 플래그가 활성화 되어있다면

        if((newfp = fopen(c_filename, "r+")) == NULL) {
        	fprintf(stderr, "creat error for %s\n", c_filename); //해당 파일 내용을 버퍼에 담기위해 읽기모드로 연다
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }
        fread(c_buffer, BUFFER_SIZE, 1 ,newfp); //버퍼에 해당 내용을 담는다
        fclose(newfp);

        if((newfp = fopen(c_filename, "w+")) == NULL) {
        	fprintf(stderr, "creat error for %s\n", c_filename); //그리고 다시 해당 파일을 연다
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }

        if((stackc_fp = fopen("Stack.c", "w+")) == NULL) { // Stack.c이라는 파일을 하나 만들어둔다
        	fprintf(stderr, "creat error for Stack.c\n");
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }

        
        strcpy(c_buffer_backup, c_buffer); //버퍼에 내용을 따로 담아둘 백업용 배열을 만들어 그곳에 집어 넣는다.
        ptr = strtok(c_buffer, "\n"); //버퍼를 개행문자 기준으로 하나씩 읽어들인다
        while(ptr != NULL){
            
            c_buffer_length = strlen(ptr); //해당 문자열의 길이
            total_length += c_buffer_length; //총 문자열의 길이, 메인함수를 만나기 전까지의 모든 문자 수들을 알기 위함
           

            if(strstr(ptr,"int main(void){") != NULL){
                break; // 메인함수를 만나면 반복문 탈출
            }

            fprintf(stackc_fp,"%s\n", ptr); // stack.c 파일에 내용을 하나씩 넣는다
            ptr = strtok(NULL,"\n"); // 다음 문자열로 이동

            
        }

        strcpy(c_buffer, c_buffer_backup); //백업버퍼에 넣어둔 내용을 다시 원래 버퍼에 넣는다
        
        fprintf(newfp, "%s", c_buffer + idx); // 버퍼에 내용을 파일에 넣는다
         fclose(newfp);
        fclose(stackc_fp);
        
    }

}

void createfile_java_to_c(int* opt_flag) {

	int length;
	int line_count = 0;
	/**/
	strcpy(filename, filename_backup); //자바파일이름을 알아온다

	if((fp = fopen(filename, "r")) == NULL) { //자바파일을 읽기전용으로 연다
		fprintf(stderr, "open error for %s\n",filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

    if((newfp = fopen(c_filename, "r")) == NULL) { // c파일을 읽기전용으로 연다
		fprintf(stderr, "open error for %s\n",c_filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

	fread(buffer, BUFFER_SIZE,1,fp); //자바파일 내용을 버퍼에 담는다
    fread(c_buffer, BUFFER_SIZE,1,newfp); // c파일 내용을 버퍼에 담는다

	fseek(fp, 0 , SEEK_SET);
    fseek(newfp, 0 , SEEK_SET);
	
	   
	if(opt_flag[2] == 1) { //option f
		        
        if(stackc_flag == 1) {

            if((stackc_fp = fopen("Stack.c","r")) == NULL){ //Stack.c 파일을 오픈한다
            fprintf(stderr, "No Stack.c file\n"); //없으면 에러 출력
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
            }

            else {
            fclose(stackc_fp);
            }
        }
	}

	fseek(fp, 0 , SEEK_SET);
    fseek(newfp, 0 , SEEK_SET);
	
	fclose(fp);
    fclose(newfp);
	
} 

void find_header(int* opt_flag) { //헤더테이블을 참조하여 헤더를 넣기위한 함수

    char* ptr;
    char* sub_ptr;
    char tmp_str[150] = "\0";
    int include_sets_length;
    int c_buffer_length;
    int stackc_length;
    int idx = 0, jdx = 0;
    int total_length = 0;

    char include_buffer[5][100];
    char include_sets_buffer[100] = "\0";
    char c_buffer_backup[BUFFER_SIZE] = "\0";


    if((newfp = fopen(c_filename, "w+")) == NULL) { // c파일을 쓰기 및 수정모드로 오픈한다
		fprintf(stderr, "open error for %s\n",c_filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}


    if((headfp = fopen("header", "r")) == NULL) { // 헤더파일을 읽기모드로 오픈한다
		fprintf(stderr, "open error for %s\n","header");
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

    
  
    fread(c_buffer, BUFFER_SIZE, 1, newfp); //버퍼에 c파일 내용을 담는다
    fread(header_buffer, BUFFER_SIZE, 1, headfp); //버퍼에 헤더 내용을 담는다

    
    if(strstr(c_buffer, "printf(") != NULL) { //printf를 찾으면 헤더플래그0을 활성
        header_flag[0] = 1;
 
    }

    if(strstr(c_buffer, "scanf(") != NULL) { //scanf를 찾으면 헤더플래그1을 활성
        header_flag[1] = 1;
    }

    if(strstr(c_buffer, "open(") != NULL) { //open를 찾으면 헤더플래그2을 활성
        header_flag[2] = 1;
    }

    if(strstr(c_buffer, "read(") != NULL) { //read를 찾으면 헤더플래그3을 활성
        header_flag[3] = 1;
    }

    if(strstr(c_buffer, "exit(") != NULL) { //exit를 찾으면 헤더플래그4을 활성
        header_flag[4] = 1;
    }


    idx = 0;
    ptr = strtok(header_buffer, "\n"); //헤더 테이블의 내용을 한줄씩 읽어들인다

    while(ptr != NULL) {
      
        strcpy(include_buffer[idx], ptr); //헤더내용을 버퍼에 담는다.
        
        ptr = strtok(NULL, "\n");
        idx++;

    }

    for(int i = 0 ; i < 5 ; i++) {
        
        if(header_flag[i]) { //헤더플래그가 활성화 되었을 시 이 문장을 수행한다.

            if(i == 2) { //만약 헤더가 3개이상 있는 파일의 경우
                ptr = strtok(include_buffer[i], "#"); // #을 기준으로 토큰화 한다

                for(i = 0 ;i < 3 ; i++) {
                ptr = strtok(NULL, "#");
                strcpy(tmp_str, ptr);
                sprintf(include_buffer[i], "%s%s     %s", "#",tmp_str,"\n"); //헤더만을 골라내서 추출한다
                strcat(include_sets_buffer, include_buffer[i]); //헤더들을 모아놓은 배열에 하나씩 중첩시킨다
                }
            }

            else { 
            ptr = strtok(include_buffer[i], "#");
            ptr = strtok(NULL, "#");
            strcpy(tmp_str, ptr);
            sprintf(include_buffer[i], "%s%s       %s", "#",tmp_str,"\n"); //헤더만을 골라내서 추출한다
            strcat(include_sets_buffer, include_buffer[i]); //헤더들을 모아놓은 배열에 하나씩 중첩시킨다.
            }
        }

        if(header_flag[0] == 1) //헤더플래그0이 활성화 되면 
            header_flag[1] = 0; //헤더플래그1을 비활성화 시킴 (printf와 scanf는 같은 헤더를 참조하므로 중복입력되지 않도록 한다.)
        
        if(header_flag[1] == 1) //헤더플래그 1이 활성화되면
            header_flag[0] = 0; //헤더플래그 0을 비활성화 시킴 (printf와 scanf는 같은 헤더를 참조하므로 중복입력되지 않도록 한다.)

        header_flag[i] = 0; //작업이 끝난 헤더플래그는 비활성화 시킨다.
    }

    if(stackc_flag) { //스택 플래그 활성화 시

        if((stackc_fp = fopen("Stack.c", "r")) == NULL) { // Stack.c을 읽기모드로 오픈한다
		fprintf(stderr, "open error for %s\n","Stack.c");
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	    }
        fread(stackc_buffer, sizeof(stackc_buffer), 1, stackc_fp); // Stack.c을 버퍼에 담는다
        fclose(stackc_fp); //파일을 닫는다.

        if((stackc_fp = fopen("Stack.c", "w+")) == NULL) { // Stack.c을 쓰기 및 수정모드로 오픈한다
		fprintf(stderr, "open error for %s\n","Stack.c");
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	    }

        
        ptr = strtok(c_buffer, "\n"); // c파일의 내용을 한줄씩 읽어본다
        while(ptr != NULL) {

            if(main_flag) { //메인함수 플래그 활성화 시
                sprintf(tmp_str, "%s%s", ptr, "\n");
                strcat(c_buffer_backup, tmp_str); //내용을 백업버퍼에 담는다. 
                strcpy(tmp_str, "\0"); //그리고 임시파일은 비워둔다
                
            }


            if(strstr(ptr, "int main") != NULL) { //메인 함수를 만났을 시
                
                sprintf(tmp_str, "\n\n%s%s", ptr, "\n");
                strcpy(c_buffer_backup, tmp_str); //해당 내용부터 백업버퍼에 담는다.
                main_flag = 1; //메인함수 플래그를 활성화 시킨다
                strcpy(tmp_str, "\0");
                
            }

            ptr = strtok(NULL, "\n");
        }
        strcpy(c_buffer, c_buffer_backup); //백업버퍼에 담았던 내용들을 원래버퍼에 새로 덮어씌운다.
        
        
        char stack_header[] = "#include \"Stack.c\""; // Stack.c를 참조한다는 의미의 헤더를 넣는다
        stackc_length = strlen(stackc_buffer);  //Stack.c 안의 문자들 수
        include_sets_length = strlen(include_sets_buffer); //헤더내용들이 담긴 문자의 수
        c_buffer_length = strlen(c_buffer); // 버퍼에 담긴 문자의 수

        

        for(int i = stackc_length - 1 ; i >= 0 ; --i) { //헤더를 넣기위해 파일내용들을 헤더문자열 길이만큼 민다.
        stackc_buffer[i + include_sets_length] = stackc_buffer[i]; 
        }

        for(int i = 0 ; i< include_sets_length ; ++i) { 
        stackc_buffer[i] = include_sets_buffer[i]; //헤더를 추가한다. (스택파일에 라이브러리가 들어갈 예정)
        }

        for(int i = c_buffer_length ; i >= 0 ; --i) {
        c_buffer[i + strlen(stack_header)] = c_buffer[i]; //헤더를 넣기위해 파일내용들을 헤더문자열 길이만큼 민다.
        
        }

        for(int i = 0 ; i < strlen(stack_header); ++i) {
        c_buffer[i] = stack_header[i]; //헤더를 추가한다 . "Stack.c" 헤더를 추가할 예정
        }


        fprintf(newfp, "%s", c_buffer); // c파일에 내용을 작성
        fprintf(stackc_fp, "%s", stackc_buffer); //stack.c 파일에 내용을 작성
        fclose(stackc_fp);
    }

    else {
    include_sets_length = strlen(include_sets_buffer); //헤더버퍼에 담긴 총 문자 수
    c_buffer_length = strlen(c_buffer); //버퍼에 있는 총 문자 수

    for(int i = c_buffer_length - 1 ; i > 0 ; --i) {
        c_buffer[i + include_sets_length] = c_buffer[i]; //헤더길이 만큼 글자들을 뒤로 민다
    }

    for(int i = 0 ; i < include_sets_length ; ++i) {
        c_buffer[i] = include_sets_buffer[i]; //헤더 추가
    }

    fprintf(newfp, "%s", c_buffer); //버퍼의 내용을 새로 생성한 c파일에 넣는다.
    }

    fclose(headfp);
    fclose(newfp);

}

void option_java_to_c(int* opt_flag) {

    int line_count = 0;

     if(opt_flag[2] == 1 && stackc_flag) { //option f
    stat("Stack.c", &statbuf);
    printf("%s file size is %ld bytes.\n", "Stack.c", statbuf.st_size); // Stack.c파일 용량 출력
    }

    if(opt_flag[2] == 1) { //option f
        stat(c_filename, &statbuf);
        printf("%s file size is %ld bytes.\n", c_filename, statbuf.st_size); // c파일 용량 출력
        stat(filename, &statbuf);
		printf("%s file size is %ld bytes.\n", filename, statbuf.st_size); //자바파일 용량 출력
    }

    if((newfp = fopen(c_filename, "r")) == NULL) {
        	fprintf(stderr, "creat error for %s\n", c_filename); //해당 파일 내용을 버퍼에 담기위해 읽기모드로 연다
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
    }

    if((fp = fopen(filename, "r")) == NULL) {
        	fprintf(stderr, "creat error for %s\n", filename); //해당 파일 내용을 읽기위해 읽기모드로 연다
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
    }

    if(stackc_flag) {
        if((stackc_fp = fopen("Stack.c", "r")) == NULL) { // Stack.c을 읽기모드로 연다
		fprintf(stderr, "open error for %s\n","Stack.c");
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	    }
    }

    if(opt_flag[3] == 1 && stackc_flag) { //option l
    fseek(stackc_fp, 0 , SEEK_SET);

    line_count = 0;
    while (!feof(stackc_fp)){//파일의 끝을 만나지 않았다면 반복
        	    ch = fgetc(stackc_fp);//파일에서 하나의 문자를 읽음
        		    if (ch == '\n')//개행 문자일 때
        		    {
            	    line_count++;//라인 번호 1 증가
        		    }
    		    }
    printf("%s line is %d lines\n", "Stack.c", line_count); //라인 수 출력
    }

    if(opt_flag[3] == 1) { //option l
    fseek(newfp, 0 , SEEK_SET);
    fseek(fp, 0 , SEEK_SET);
    
    line_count = 0;
    while (!feof(fp)){//파일의 끝을 만나지 않았다면 반복
        	ch = fgetc(fp);//파일에서 하나의 문자를 읽음
        		if (ch == '\n')//개행 문자일 때
        		{
            	line_count++;//라인 번호 1 증가
        		}
    		}
		
		printf("%s line is %d lines\n", filename, line_count); // 자바파일 라인 수 출력
        line_count = 0;

    line_count = 0;

        while (!feof(newfp)){//파일의 끝을 만나지 않았다면 반복
        	ch = fgetc(newfp);//파일에서 하나의 문자를 읽음
        		if (ch == '\n')//개행 문자일 때
        		{
            	line_count++;//라인 번호 1 증가
        		}
    		}
		
		printf("%s line is %d lines\n", c_filename, line_count); // c파일 라인 수 출력
    }

    if(opt_flag[0] == 1) { //option j
		printf("%s", buffer); //자바파일 내용 보기
	}

    if(opt_flag[1] == 1 && stackc_flag) {   //c 옵션 활성화 시 그리고 스택플래그 활성화 시
        printf("%s", stackc_buffer); // stack.c 내용 출력
        printf("%s", c_buffer); // c파일 내용 출력

    }

    else if(opt_flag[1] == 1) {   //c 옵션 활성화 시 
        printf("%s", c_buffer); // c파일 내용 출력

    }

    fclose(fp);
    fclose(newfp);

    if(stackc_flag) { //stack 플래그가 설정되어있으면
    fclose(stackc_fp); //스택파일을 닫는다.
    }

}

void create_makefile() { // makefile을 만들기 위한 함수

    char *filename_ptr; //파일이름(.앞에 붙는 이름)
    char makefile_name[30];
    char makefile_command[40];

    filename_ptr = strtok(filename,".");
    sprintf(makefile_name, "%s%s%s", filename_ptr, "_", "Makefile");

    if((makefp = fopen(makefile_name, "w+")) == NULL) { //makefile 생성
		fprintf(stderr, "open error\n");
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

    sprintf(makefile_command, "%s %s%s%s%s %s %s %s","gcc",":","\n","\t","gcc", c_filename , "-o", filename_ptr);

    fprintf(makefp,"%s",makefile_command); // ssu_convert.c 컴파일
    fclose(makefp);
}



