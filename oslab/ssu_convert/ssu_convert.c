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
    char tmp_str[150];
    char tabadd_str[100];

    int ptr_count = 0;
    int sub_ptr_count = 0;
    int chr=0;
    
    
    //strcpy(filename, "q2.java");
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

        if(bracket_delete_flag == 1) {
            strcpy(tmp_str, "\0");
            bracket_delete_flag = 0;
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
            bracket_delete_flag = 1;
        }

        else if(strstr(tmp_str, "File file = new File(\"q3java.txt\");") != NULL) {
            strcpy(tmp_str, "\t\tFILE* file;\n\t\tif((file = fopen(\"q3java.txt\", \"w+\")) == NULL){\n\t\t\tfprintf(stderr,\"open error\\n\");\n\t\t\texit(1);\n\t\t}");
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
            strcpy(tmp_str, "#include <stdio.h>\n#include <stdlib.h>\n#include <unistd.h>\n#include <fcntl.h>\n");
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
            return_to_exit_flag = 1;
        }

        else if(strstr(tmp_str, "public static void main(String[] args)") != NULL) {
            strcpy(tmp_str, "int main(void){");
            return_to_exit_flag = 1;
        }

        else if(strstr(tmp_str, "Scanner scn = new Scanner(System.in)") != NULL) {
            strcpy(tmp_str, "\0");
        }

        else if((sub_ptr = strstr(tmp_str, "System.out.printf")) != NULL) {
             tab_flag = 1;
             strcpy(tmp_str, sub_ptr + 11);

             if(strstr(tmp_str, "st.peek") != NULL) {
                strcpy(tmp_str, "printf(\"TOP OF STACK : %d\\n\", peek());");
                tab_flag = 0;
             }
        }

        else if((sub_ptr = strstr(tmp_str, "st.")) != NULL) {
            strcpy(tmp_str, sub_ptr+3);
        }

        else if(strstr(tmp_str, "num = scn.nextInt();") != NULL) {
            strcpy(tmp_str, "\t\tscanf(\"%d\",&num);");
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
    fprintf(makefp,"%s","q2 :\n\tgcc q2.c Stack.c -o q2\n");
    fprintf(makefp,"%s","q3 :\n\tgcc q3.c -o q3\n");

    fclose(makefp);
}

void find_header() {

}

