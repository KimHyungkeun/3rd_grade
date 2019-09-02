#include "ssu_convert.h"
#include "ssu_time.h"

int main(void) {

    char *filename_ptr;
    char *ptr;
    char *tmp_ptr;
    char *sub_ptr;
    char tmp_str[100];

    int ptr_count = 0;
    int sub_ptr_count = 0;
    int chr=0;
    int stackc_flag = 0;
    
    strcpy(filename, "q2.java");
    if((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "open error\n");
		exit(1);
	}

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
        printf("TOKEN : %s\n", tmp_str);

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
       
        printf("ptr_count : %d\n", ptr_count);
        ptr_count ++;
        ptr = strtok(NULL, "\n");    
    }
       
    fclose(newfp);
    fclose(fp);


    if(stackc_flag == 1) {

        printf("%s\n",c_filename);
        if((stackc_fp = fopen(c_filename, "r")) == NULL) {
        	fprintf(stderr, "creat error\n");
            exit(1);
        }

        fread(c_buffer, BUFFER_SIZE, 1 ,stackc_fp);
        printf("%s\n", c_buffer);
	    fseek(stackc_fp, 0 , SEEK_SET);
        fclose(stackc_fp);

        if((stackc_fp = fopen("Stack.c", "w+")) == NULL) {
        	fprintf(stderr, "creat error\n");
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

    exit(0);

}