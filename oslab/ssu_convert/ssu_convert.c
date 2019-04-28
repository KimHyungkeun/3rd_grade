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
    option_java_to_c(opt_flag); //옵션 처리
    find_header(opt_flag); //헤더파일 찾기
    create_makefile(); //makefile 만들기
    
	
	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void convert_java_to_c(int* opt_flag) {

    char *filename_ptr;
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
    
    
    
    if((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "open error for %s\n", filename);
		exit(1);
	}

    strcpy(filename_backup, filename);
    
    filename_ptr = strtok(filename,".");
    sprintf(c_filename,"%s%s",filename_ptr,".c");
    if((newfp = fopen(c_filename, "w+")) == NULL) {
        	fprintf(stderr, "open error for %s\n", c_filename);
            exit(1);
    }
    
    fread(buffer, BUFFER_SIZE,1,fp);
	fseek(fp, 0 , SEEK_SET);
  
  
    while(buffer[chr] != '\n') {
        chr++;
    }
    
   
    
     printf("%s converting is finished!\n",c_filename);


    ptr = strtok(buffer, "\n");
    while (ptr != NULL)              
    {
        strcpy(tmp_str, ptr);
        

        if(strcmp(ptr,"}") == 0) {
            strcpy(tmp_str, "\0");
        }

        if(bracket_delete_flag == 1) {
            strcpy(tmp_str, "\0");
            bracket_delete_flag = 0;
        }

        
        
        if(strstr(tmp_str, "import java.util.Scanner;") != NULL) {
           
            strcpy(tmp_str, "\0");
        }

        else if(strstr(tmp_str, "import java.io.File;") != NULL) {
            
            strcpy(tmp_str, "\0");
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
            bracket_delete_flag = 1;
        }

        else if(strstr(tmp_str, "File file = new File") != NULL) {
            strcpy(tmp_str, "\t\tFILE* file;\n\t\tif((file = fopen(\"q3java.txt\", \"w+\")) == NULL){\n\t\t\tfprintf(stderr,\"open error\\n\");\n\t\t\texit(1);\n\t\t}");
            if(opt_flag[4] == 1) {
                printf("File file -> fopen\n");
            }
        }

        else if(strstr(tmp_str, "FileWriter writer = new FileWriter(file, false);") != NULL) {
            strcpy(tmp_str, "\0");
            if(opt_flag[4] == 1) {
                printf("FileWriter -> fprintf\n");
            }
        }

        else if(strstr(tmp_str, "writer.write(\"2019 OSLAB\\n\");") != NULL) {
            
            strcpy(tmp_str, "\t\tfprintf(file, \"%s\" ,\"2019 OSLAB\\n\");");
        }

        else if(strstr(tmp_str, "writer.write(\"Linux System Programming\\n\");") != NULL) {
             
            strcpy(tmp_str, "\t\tfprintf(file, \"%s\" ,\"Linux System Programming\\n\");");
        }
        
        else if(strstr(tmp_str, "writer.flush();") != NULL) {
            strcpy(tmp_str, "\t\tfflush(stdin);");
            if(opt_flag[4] == 1) {
                printf("writer.flush() -> fflush(stdin)\n");
            }
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
            
            strcpy(tmp_str, "\0");
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

        else if(strstr(tmp_str, "public static void main") != NULL) {
            strcpy(tmp_str, "int main(void){");
            return_to_exit_flag = 1;
            if(opt_flag[4] == 1) {
                printf("public static void main -> int main(void)\n");
            }
        }

        else if(strstr(tmp_str, "Scanner scn = new Scanner(System.in)") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if((sub_ptr = strstr(tmp_str, "System.out.printf")) != NULL) {
             tab_flag = 1;
             strcpy(tmp_str, sub_ptr + 11);

            if(opt_flag[4] == 1 && print_count < 1) {
                printf("System.out.printf -> printf\n");
                print_count++;
            }

             if(strstr(tmp_str, "st.peek") != NULL) {
                strcpy(tmp_str, "printf(\"TOP OF STACK : %d\\n\", peek());");
                tab_flag = 0;
             }

             
        }

        else if((sub_ptr = strstr(tmp_str, "st.")) != NULL) {
            strcpy(tmp_str, sub_ptr+3);
        }

        else if(strstr(tmp_str, "scn.nextInt();") != NULL) {
            strcpy(tmp_str, "\t\tscanf(\"%d\",&num);");
            if(opt_flag[4] == 1) {
                printf("scn.nextInt -> scanf\n");
            }
        }

        else if(strstr(tmp_str, "return") != NULL) {
            if(return_to_exit_flag == 1) {
            strcpy(tmp_str, "\t\texit(0);");
            return_to_exit_flag = 0;
            }
        }


        if(tab_flag == 1) {
        fprintf(newfp, "\t\t%s\n", tmp_str);
        tab_flag = 0;
        }

        else {
        fprintf(newfp, "%s\n", tmp_str);
        }

        
        ptr_count ++;
        ptr = strtok(NULL, "\n");    
    }

    fclose(newfp);
    fclose(fp);

    if(stackc_flag == 1) {

       

        if((newfp = fopen(c_filename, "r+")) == NULL) {
        	fprintf(stderr, "creat error for %s\n", c_filename);
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }
        fread(c_buffer, BUFFER_SIZE, 1 ,newfp);
        fclose(newfp);

        if((newfp = fopen(c_filename, "w+")) == NULL) {
        	fprintf(stderr, "creat error for %s\n", c_filename);
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }

        if((stackc_fp = fopen("Stack.c", "w+")) == NULL) {
        	fprintf(stderr, "creat error for Stack.c\n");
            gettimeofday(&end_t,NULL);
    	    ssu_runtime(&begin_t, &end_t);
            exit(1);
        }

        
        strcpy(c_buffer_backup, c_buffer);
        ptr = strtok(c_buffer, "\n");
        while(ptr != NULL){
            
            c_buffer_length = strlen(ptr);
            total_length += c_buffer_length;
           

            if(strstr(ptr,"int main(void){") != NULL){
                break;
            }

            fprintf(stackc_fp,"%s\n", ptr);
            ptr = strtok(NULL,"\n");

            
        }

        strcpy(c_buffer, c_buffer_backup);
        
        fprintf(newfp, "%s", c_buffer + idx);
         fclose(newfp);
        fclose(stackc_fp);
        
    }

    

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

        if(stackc_flag == 1)
            ;
        else
            printf("%s", c_buffer);

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
	
	

	fclose(fp);
    fclose(newfp);
	
}

void find_header(int* opt_flag) {

    char* ptr;
    char* sub_ptr;
    char tmp_str[150] = "\0";
    int include_sets_length;
    int c_buffer_length;
    int stackc_length;
    int idx = 0, jdx = 0;
    int line_count = 0;
    int total_length = 0;

    char include_buffer[5][100];
    char include_sets_buffer[100] = "\0";
    char c_buffer_backup[BUFFER_SIZE] = "\0";


    if((newfp = fopen(c_filename, "w+")) == NULL) {
		fprintf(stderr, "open error for %s\n",c_filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}


    if((headfp = fopen("header", "r")) == NULL) {
		fprintf(stderr, "open error for %s\n",c_filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

    
    //printf("%s", c_buffer);
    fread(c_buffer, BUFFER_SIZE, 1, newfp);
    fread(header_buffer, BUFFER_SIZE, 1, headfp);

    
    if(strstr(c_buffer, "printf(") != NULL) {
        header_flag[0] = 1;
 
    }

    if(strstr(c_buffer, "scanf(") != NULL) {
        header_flag[1] = 1;
    }

    if(strstr(c_buffer, "open(") != NULL) {
        header_flag[2] = 1;
    }

    if(strstr(c_buffer, "read(") != NULL) {
        header_flag[3] = 1;
    }

    if(strstr(c_buffer, "exit(") != NULL) {
        header_flag[4] = 1;
    }


    idx = 0;
    ptr = strtok(header_buffer, "\n");

    while(ptr != NULL) {
      
        strcpy(include_buffer[idx], ptr);
        
        ptr = strtok(NULL, "\n");
        idx++;

    }

    for(int i = 0 ; i < 5 ; i++) {
        
        if(header_flag[i]) {

            if(i == 2) {
                ptr = strtok(include_buffer[i], "#");

                for(i = 0 ;i < 3 ; i++) {
                ptr = strtok(NULL, "#");
                strcpy(tmp_str, ptr);
                sprintf(include_buffer[i], "%s%s       %s", "#",tmp_str,"\n");
                strcat(include_sets_buffer, include_buffer[i]);
                }
            }

            else { 
            ptr = strtok(include_buffer[i], "#");
            ptr = strtok(NULL, "#");
            strcpy(tmp_str, ptr);
            sprintf(include_buffer[i], "%s%s    %s", "#",tmp_str,"\n");
            strcat(include_sets_buffer, include_buffer[i]);
            }
        }

        if(header_flag[0] == 1)
            header_flag[1] = 0;
        
        if(header_flag[1] == 1)
            header_flag[0] = 0;

        header_flag[i] = 0;
    }

    if(stackc_flag) {

        if((stackc_fp = fopen("Stack.c", "r")) == NULL) {
		fprintf(stderr, "open error for %s\n",c_filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	    }
        fread(stackc_buffer, sizeof(stackc_buffer), 1, stackc_fp);
        fclose(stackc_fp);

        if((stackc_fp = fopen("Stack.c", "w+")) == NULL) {
		fprintf(stderr, "open error for %s\n",c_filename);
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	    }

        
        ptr = strtok(c_buffer, "\n");
        while(ptr != NULL) {

            if(main_flag) {
                sprintf(tmp_str, "%s%s", ptr, "\n");
                strcat(c_buffer_backup, tmp_str);
                strcpy(tmp_str, "\0");
                
            }


            if(strstr(ptr, "int main") != NULL) {
                
                sprintf(tmp_str, "\n\n%s%s", ptr, "\n");
                strcpy(c_buffer_backup, tmp_str);
                main_flag = 1;
                strcpy(tmp_str, "\0");
                
            }

            ptr = strtok(NULL, "\n");
        }
        strcpy(c_buffer, c_buffer_backup);
        
        
        char stack_header[] = "#include \"Stack.c\"";
        stackc_length = strlen(stackc_buffer);
        include_sets_length = strlen(include_sets_buffer);
        c_buffer_length = strlen(c_buffer);

        

        for(int i = stackc_length - 1 ; i >= 0 ; --i) {
        stackc_buffer[i + include_sets_length] = stackc_buffer[i];
        }

        for(int i = 0 ; i< include_sets_length ; ++i) {
        stackc_buffer[i] = include_sets_buffer[i];
        }

        for(int i = c_buffer_length ; i >= 0 ; --i) {
        c_buffer[i + strlen(stack_header)] = c_buffer[i];
        
        }

        for(int i = 0 ; i < strlen(stack_header); ++i) {
        c_buffer[i] = stack_header[i];
        }


        fprintf(newfp, "%s", c_buffer);
        fprintf(stackc_fp, "%s", stackc_buffer);
        fclose(stackc_fp);
    }

    else {
    include_sets_length = strlen(include_sets_buffer);
    c_buffer_length = strlen(c_buffer);

    for(int i = c_buffer_length - 1 ; i > 0 ; --i) {
        c_buffer[i + include_sets_length] = c_buffer[i];
    }

    for(int i = 0 ; i < include_sets_length ; ++i) {
        c_buffer[i] = include_sets_buffer[i];
    }

    fprintf(newfp, "%s", c_buffer);
    }

    if(opt_flag[1] == 1) {   
        printf("%s", stackc_buffer);
        printf("%s", c_buffer);

    }

    
    fclose(headfp);
    fclose(newfp);

}

void create_makefile() {

    if((makefp = fopen("Makefile", "w+")) == NULL) {
		fprintf(stderr, "open error\n");
		gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
		exit(1);
	}

    fprintf(makefp,"%s","gcc :\n\tgcc ssu_convert.c -o ssu_convert\n");
    fprintf(makefp,"%s","ssu_convert1 :\n\t./ssu_convert q1.java\n");
    fprintf(makefp,"%s","ssu_convert2 :\n\t./ssu_convert q2.java\n");
    fprintf(makefp,"%s","ssu_convert3 :\n\t./ssu_convert q3.java\n");
    fprintf(makefp,"%s","q1 :\n\tgcc q1.c -o q1\n");
    fprintf(makefp,"%s","q2 :\n\tgcc q2.c -o q2\n");
    fprintf(makefp,"%s","q3 :\n\tgcc q3.c -o q3\n");

    fclose(makefp);
}



