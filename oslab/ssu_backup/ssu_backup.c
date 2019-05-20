#include "ssu_backup.h"

int main(int argc, char* argv[]) {

   
   char mkdir_command[BUFFER_MAX];


    if(argc > 2) {
         fprintf(stderr, "Usage : %s [directory path]\n", argv[0]);
          exit(1);
    }

    if(argv[1] == NULL) {
         strcpy(directory_name, "ssu_backup_directory");
         sprintf(mkdir_command, "%s %s", "mkdir", directory_name);
         system(mkdir_command);
    }

    else{
           strcpy(directory_name, argv[1]);
           sprintf(mkdir_command, "%s %s", "mkdir", directory_name);
           system(mkdir_command);
    }

    //printf("%s\n", directory_name);

    stat(directory_name, &statbuf);

    if(access(directory_name, F_OK) < 0) {
         fprintf(stderr, "Usage : %s [directory path]\n", argv[0]);
         exit(1);
    }

    if(!S_ISDIR(statbuf.st_mode)) {
         fprintf(stderr, "Usage : %s [directory path]\n", argv[0]);
         exit(1);
    }

    

   prompt_environment(); //프롬프트 

   exit(0);

}

void prompt_environment(void) {

    Backup_list list;

    while(1) {
        fputs(prompt, stdout);

        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = 0;

        if (strstr(command,"vi") != NULL) { //vi 명령
             system(command); //명령 실행
             continue;
        }

        else if (strstr(command,"ls") != NULL) { //list 명령
             system(command); //명령 실행
             continue;
        }
           

        else if (strstr(command,"add") != NULL) { //add 명령
            printf("add is executed\n");
            add_function();
            continue;
        }

        else if (strstr(command,"remove") != NULL) { //remove 명령
             printf("remove is executed\n");
             remove_function();
             continue;
        }

        else if (strstr(command,"compare") != NULL) { //compare 명령
             printf("compare is executed\n");
             continue;
        }

        else if (strstr(command,"recover") != NULL) { //recover 명령
             printf("recover is executed\n");
             continue;
        }


        else if (strstr(command,"list") != NULL) { //list 명령
             printf("list is executed\n"); //명령 실행
             continue;
        }

        else if (strstr(command,"exit") != NULL) { //프롬프트 종료 명령
            break;
        }

        else if (strcmp(command,"\n") == 0) {
            continue;
        }

        else {
            printf("Unknown Command\n");  //프롬프트 종료 명령
            continue;
        }

    }

    //fprintf(stdout, "Bye~!\n");
    //fflush(stdout);
}

void add_function() {
     char *ptr;
     char filename[BUFFER_MAX];
     char* command_token[10];
     int i = 0;

     int opt;
     int opt_flag[4] = {0,0,0,0}; //옵션 플래그

     ptr = strtok(command, " ");

    while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    {
        command_token[i] = ptr;          // 자른 문자열 출력
        printf("%s\n", command_token[i]);
        printf("i : %d\n", i);
        i++;  
        printf("Checkpoint\n");
        ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
    }

    strcpy(filename, command_token[1]);

    while ((opt = getopt(i+1, command_token, "mntd")) != -1) { //옵션은 m,n,t,d 4종류가 있다
    
    switch(opt) {

        case 'm' : // 옵션
            opt_flag[0] = 1; //mtime 수정 되었을 경우 백업 실행
            break;
         
        case 'n' : // 옵션
            opt_flag[1] = 1; //백업한 파일의 최대 갯수
            break;
        
        case 't' : // 옵션
            opt_flag[2] = 1; //백업해야할 파일에 대한 백업 디렉터리 내 보관 기간을 time만큼 설정
            break;

        case 'd' : // 옵션
            opt_flag[3] = 1; //지정한 디렉터리 내의 모든 파일들을 백업 리스트에 추가
            break;
		
    }
   }
   

}

void remove_function() {
     char *ptr;
     char filename[BUFFER_MAX];
     char* command_token[3];
     int i = 0;

     int opt;
     int opt_flag = 0; //옵션 플래그

     ptr = strtok(command, " ");

    while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    {
        command_token[i] = ptr;          // 자른 문자열 출력
        printf("%s\n", command_token[i]);
        printf("i : %d\n", i);
        i++;  
        ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
    }

    strcpy(filename, command_token[1]);
     
     while ((opt = getopt(i+1, command_token, "mntd")) != -1) { //옵션은 a 1종류가 있다
          switch(opt) {

          case 'a' : // 옵션
            opt_flag = 1; //백업 리스트에 존재하는 파일의 백업을 중단하기 위해 백업 리스트에서 삭제
            break;

          case '?' : // 옵션
            printf("Wrong Option\n");
            break;
          
          }
  
     }
}