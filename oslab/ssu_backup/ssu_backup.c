#include "ssu_backup.h"

int main(int argc, char* argv[]) {

   
   char mkdir_command[BUFFER_MAX]; //백업디렉터리 이름을 담을 배열
   char logmake_command[BUFFER_MAX]; //로그파일 이름을 담을 배열

    strcpy(logfile, "log.txt"); 

    if(argc > 2) {
         fprintf(stderr, "Usage : %s [directory path]\n", argv[0]); //인자가 부족하면 에러를띄우고 프로그램 종료
          exit(1);
    }

    if(argv[1] == NULL) {
         strcpy(backup_dir, "ssu_backup_dir"); //ssu_backup_dir가 디폴트 백업디렉토리이다
         

         if(access(backup_dir, F_OK) == 0) //이미 존재하면 아무것도 하지 않는다
                        ;
         

         else {
         sprintf(mkdir_command, "%s %s", "mkdir", backup_dir);
         system(mkdir_command); //만약 없다면 새로 생성한다
         }
            
    }

    else{
           strcpy(backup_dir, argv[1]);
           sprintf(mkdir_command, "%s %s", "mkdir", backup_dir); //사용자가 지정한 이름으로 백업디렉터리를 생성한다
           system(mkdir_command);
    }

    
    stat(backup_dir, &statbuf); 

    if(access(backup_dir, F_OK) < 0) {
         fprintf(stderr, "Usage : %s [directory path]\n", argv[0]); //해당 디렉터리가 존재하지 않으면 프로그램 종료
         exit(1);
    }

    if(!S_ISDIR(statbuf.st_mode)) {
         fprintf(stderr, "Usage : %s [directory path]\n", argv[0]); //해당 파일이 디렉터리 파일이 아니면 프로그램 종료
         exit(1);
    }

    

   prompt_environment(); //프롬프트 환경 활성화
 
   exit(0);

}

void list_function(Backup_list* head) { //list 함수

    Backup_list* listcurr;
    listcurr = head -> next; //헤드의 다음노드부터 백업 리스트이다.

    if(head -> next == NULL) //헤드이외에 아무노드도 없다면 함수 종료
        return;

    while(listcurr != NULL) {
        printf("%s     %d\n", listcurr -> filepath, listcurr -> period); //존재하는 리스트들을 모두 출력한다.
        listcurr = listcurr -> next;
    }
    
}

void prompt_environment(void) { //prompt 환경

    int idx = 0;
    struct tm *tm_p;
    time_t now;
    Backup_list *head;
    Backup_list *curr;
    char *prompt = "20142332>";
    char backdir_fullpath[BUFFER_MAX];
    
    realpath(backup_dir, backdir_fullpath); //백업 디렉터리의 절대경로를 구한다
    strcat(backdir_fullpath,"/");
    strcat(backdir_fullpath,logfile); //그리고 백업디렉터리내에 놓여질 로그파일에 대한 절대경로를 구한다
 
   
    if((log_fp = fopen(backdir_fullpath, "w")) == NULL) {
        fprintf(stderr, "fopen error\n"); //로그파일을 오픈한다
        return;
    }
    setbuf(log_fp, NULL);
   

    head = (Backup_list*)malloc(sizeof(Backup_list)); //헤드노드를 생성한다. 이 헤드노드를 기점으로 백업리스트들이 삭제 또는 추가된다.
    curr = head;
    curr -> next = NULL;
    curr -> prev = NULL;

    while(1) {

        time(&now);
        tm_p = localtime(&now);

        if(recover_flag == 0)
            fputs(prompt, stdout); //recover가 수행되지 않으면 프롬프트를 띄운다
        else {
            recover_flag = 0; //수행되었을때는 띄우지 않는다. (recover 정상 작동 후에 프롬프트가 2번 띄워지는 현상 발생)
        }
        
        fgets(command, sizeof(command), stdin); //커맨드 입력
        command[strlen(command) - 1] = 0; 
        strcpy(command_tmp, command); //커맨드 구문 백업

        if (strstr(command,"vi") != NULL) { //vi 명령
             system(command); //명령 실행
             continue;
        }

        else if (strstr(command,"ls") != NULL) { //list 명령
             system(command); //명령 실행
             continue;
        }
           

        else if (strstr(command,"add") != NULL) { //add 명령
            
            if(strcmp(command,"add") == 0) {
                fprintf(stderr, "Usage : %s FILENAME [PERIOD] [OPTION]\n", command); //명령어 이외의 다른 인자가 없으면 다시 프롬프트로 돌아감
                continue;
            }

            if(add_command_analyzer(head) != 0) 
                continue;
           
            curr -> next = (Backup_list*)malloc(sizeof(Backup_list)); //백업리스트를 새로 추가
            
            
            if(pthread_create(&curr -> next -> tid, NULL ,add_function, (void *)curr) != 0) { //백업파일 하나당 쓰레드를 생성.
                fprintf(stderr, "thread_create error\n");
                continue;
            }
            
            curr = curr -> next; //모든 작업이 완료되면 새로운 리스트 할당 준비를 위해 다음 노드를 가리킨다
            continue;
        }

        else if (strstr(command,"remove") != NULL) { //remove 명령
             
             if(strcmp(command,"remove") == 0) {
                fprintf(stderr, "Usage : %s FILENAME [OPTION]\n", command); //명령어 이외의 다른 인자가 없으면 다시 프롬프트로 돌아감
                continue;
             }

             curr = remove_function(head);
             continue;
        }

        else if (strstr(command,"compare") != NULL) { //compare 명령
             
             if(strcmp(command,"compare") == 0) {
                 fprintf(stderr, "Usage : %s FILENAME1 FILENAME2\n", command); //명령어 이외의 다른 인자가 없으면 다시 프롬프트로 돌아감
                 continue;
             }
             compare_function();
             continue;
        }

        else if (strstr(command,"recover") != NULL) { //recover 명령
             
             if(strcmp(command,"recover") == 0) {
                 fprintf(stderr, "Usage : %s FILENAME [OPTION]\n", command); //명령어 이외의 다른 인자가 없으면 다시 프롬프트로 돌아감
                 continue;
             }
             
             curr = recover_function(head);
             continue;
        }


        else if (strstr(command,"list") != NULL) { //list 명령
             
             list_function(head);
             continue;
        }

        else if (strstr(command,"exit") != NULL) { //프롬프트 종료 명령
            free(head);
            break;
        }

        else if (strcmp(command,"\0") == 0) { //단순히 엔터키를 누르면 바로 프롬프트를 띄우게 한다
            continue;
        }

        else {
            printf("Unknown Command\n");  //유효하지 않은 명령어 입력시 메세지를 띄우고 다시 프롬프트로 돌아간다.
            continue;
        }

        

    }

    fclose(log_fp); //로그파일 닫음

}


int add_command_analyzer(Backup_list* head) {

    struct tm *tm_p;
    struct tm time_struct;
    time_t now;

    char *ptr;
    char *command_token[10];
    char filename_buf[BUFFER_MAX];
    char period[4];
    int i = 0;
    int num;
  
    Backup_list *curr;
    ptr = strtok(command, " ");

    while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    {
        command_token[i] = ptr;          // 자른 문자열 출력
        i++;  
        
        ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
    }

    strcpy(filename, command_token[1]); //파일이름을 담는다
    stat(filename, &statbuf);
    if(i < 3) {
        fprintf(stderr,"Usage : %s FILENAME [PERIOD] [OPTION]\n",command_token[0]); //만약 PERIOD까지 적지 않았다면 프롬프트로 돌아감
        return 1;
    }
 
    if(access(filename, F_OK) < 0) {
        fprintf(stderr,"\"%s\" does not exist\n", filename); //백업하려는 파일이 존재하지 않으면 에러 후 프롬프트로 돌아감
        return 1;
    }

    if (!S_ISREG(statbuf.st_mode)) {
        fprintf(stderr,"Not regular file\n"); //일반파일이 아니면 에러 처리 후 프롬프트로 돌아감
        return 1;
    }

    curr = head;
    while (curr != NULL) {
        if(strcmp(curr -> filepath, realpath(filename,filename_buf)) == 0 ) {
            fprintf(stderr,"\"%s\" is already exist in backup list.\n", filename); //백업리스트에 파일이 이미 존재하면 에러처리 후 프롬프트로 돌아감
            return 1;
        }
        curr = curr -> next;
    }

    strcpy(period, command_token[2]); //PERIOD로 돌아간다.
  
    

    if (strstr(period,".") != NULL) {
        fprintf(stderr,"%s is not Integer. Try again\n", period); //입력한 PERIOD가 실수이면 에러처리후 프롬프트로 돌아간다
        return 1;
    }


    if(atoi(period) < 5 || atoi(period) > 10) {
        fprintf(stderr,"Please retry (5 <= PERIOD <= 10)\n"); //PERIOD가 5초미만이거나 10초 초과시 에러처리후 프롬프트로 돌아간다.
        return 1;
    }

    time(&now);
    tm_p = localtime(&now); //정상적으로 잘 넘어왔을 시 아래의 내용을 로그파일에 입력한다.
    fprintf(log_fp,"[%d%02d%02d %02d%02d%02d] add is executed\n", tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    return 0;

}

void *add_function(void *arg) {
    struct tm *tm_p;
    struct tm time_struct;
    time_t now;
    int i = 0;

    char buf[BUFFER_MAX];
    char log_buf[BUFFER_MAX];
    char bck_buf[BUFFER_MAX];
    char cp_command[500];
    char period[4];

    char *strrchr_ptr;
    char *ptr;
    char *command_token[10];

    char filename_local[BUFFER_MAX];
    char command_local[BUFFER_MAX];
    char backup_local[BUFFER_MAX];

    char backup_realpath[BUFFER_MAX];
    char filename_final[BUFFER_MAX];
  
    Backup_list* listcurr;
    listcurr = (Backup_list*)arg;

    
    strcpy(command_local, command_tmp); //명령어를 이 함수 내에서만 다룰 수 있도록 지역변수를 하나 선언한다.

    ptr = strtok(command_local, " ");
    while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    {
        command_token[i] = ptr;          // 자른 문자열 출력
        i++;  
        
        ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
    }

    strcpy(period, command_token[2]); //PERIOD를 담음
    strcpy(filename_local, filename); //파일이름을 이 함수 내 지역변수에 담음
    strcpy(backup_local, backup_dir); //백업디렉터리를 이 함수 내 지역변수에 담는다/
    
    listcurr -> next -> prev = listcurr; 
    listcurr -> next -> pid = getpid();  //프로세스 아이디 넣음
    strcpy(listcurr -> next ->  filepath, realpath(filename_local,buf)); // 파일의 절대경로를 넣음
    listcurr -> next -> period = atoi(period); //주기를 넣음. 
    
    listcurr -> next -> next = NULL;

    strrchr_ptr = strrchr(filename_local, '/'); //파일의 절대경로 중에서 파일이름만을 뽑아낸다.

    if (strrchr_ptr != NULL)
        strcpy(filename_final, strrchr_ptr+1); //절대경로를 입력했을 시 잘라내는 방식
    else
        strcpy(filename_final, filename_local); //상대경로 또는 파일이름만 입력시 잘라내는 방식
    
    time(&now);
    tm_p = localtime_r(&now, &time_struct);
    
    sprintf(bck_buf, "%s_%d%02d%02d%02d%02d%02d", filename_final,tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    sprintf(log_buf,"[%d%02d%02d %02d%02d%02d] %s/%s_%d%02d%02d%02d%02d%02d added\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_local, backup_realpath), filename_final,tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    sprintf(cp_command, "%s %s %s/%s","cp",realpath(filename_local,buf),backup_dir,bck_buf); //백업 커맨드 구문 완성
   
    fprintf(log_fp, "%s", log_buf); //로그파일에 로그 기록
    system(cp_command); //커맨드 실행
    sleep(atoi(period)); //설정한 주기만큼 일시정지
  
    while(1) {
        time(&now);
        tm_p = localtime_r(&now, &time_struct);

        sprintf(bck_buf, "%s_%d%02d%02d%02d%02d%02d", filename_final ,tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
        sprintf(log_buf,"[%d%02d%02d %02d%02d%02d] %s/%s_%d%02d%02d%02d%02d%02d generated\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_local, backup_realpath), filename_final, tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
        sprintf(cp_command, "%s %s %s/%s","cp",realpath(filename_local,buf),backup_dir,bck_buf); //백업 커맨드 구문 완성
        
        fprintf(log_fp, "%s", log_buf); //로그파일에 로그 기록
        system(cp_command); //커맨드 실행
        sleep(atoi(period)); //설정한 주기만큼 일시 정지
    }
     
    
}

Backup_list* remove_function(Backup_list* head) {

    char *strrchr_ptr;
    char *ptr;
    char *command_token[2];
    char buf[BUFFER_MAX];
    char backup_local[BUFFER_MAX];
    char backup_realpath[BUFFER_MAX];
    char filename_local[BUFFER_MAX];

    char filename_realpath[BUFFER_MAX];
    char token_realpath[BUFFER_MAX];

    char filename_final[BUFFER_MAX];

    int i = 0;
    struct tm *tm_p; 
    struct tm time_struct;
    time_t now;
    pthread_t local_tid;
    Backup_list *listhead = head;
    Backup_list *curr;


    strcpy(backup_local, backup_dir);
    ptr = strtok(command, " ");

    while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    {
        command_token[i] = ptr;          // 자른 문자열 출력
        i++;  
        ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
    }

    strcpy(filename_local, command_token[1]);
    
    strrchr_ptr = strrchr(filename_local, '/'); //파일의 절대경로에서 파일이름만을 추출한다.


    if (strrchr_ptr != NULL)
        strcpy(filename_final, strrchr_ptr+1); //파일의 절대경로 입력시 추출하는 방법
    else
        strcpy(filename_final, filename_local); //상대경로 입력 또는 파일이름만 입력했을시 추출하는 방법

    

    if(strcmp(command_token[1], "-a") == 0) { //옵션 a를 넣었을때
        curr = listhead -> next;
        while (curr != NULL) { 
            time(&now);
            tm_p = localtime_r(&now, &time_struct);
            local_tid = curr -> tid;
            pthread_cancel(local_tid); //해당 쓰레드를 캔슬시킨다
     
            curr -> prev -> next = curr -> next;
            if(curr -> next == NULL)
                curr -> next = NULL;
            else 
                curr -> next -> prev = curr -> prev; //해당 노드의 이전과 다음 노드와의 연결을 끊어버린다.

            fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] %s/%s deleted\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_local, backup_realpath) ,strrchr(curr -> filepath, '/')+1);
            free(curr); //해당 리스트를 삭제한다.
            curr = listhead -> next;
        }
        curr = listhead; //삭제했다는 사실을 로그파일에 기록한다.
        fprintf(log_fp,"[%d%02d%02d %02d%02d%02d] remove is executed\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
        return curr;
    }

    else {
        curr = listhead -> next;
        while (curr != NULL) {
            time(&now);
            tm_p = localtime_r(&now, &time_struct);

            if(access(filename_local, F_OK) < 0) { //현 디렉터리 내에 파일이 존재하지 않는다면
                
                curr = listhead;
                while(curr != NULL) {
                    
                    if(curr -> next == NULL) {
                    break;
                    }

                curr = curr -> next; //백업리스트의 위치를 맨 마지막으로 이동시킨다.
                }
                fprintf(stderr, "Can't find \"%s\" in ssu_backup\n", filename_local); //에러메세지 출력
                return curr; //백업리스트 주소값을 반환
            }

            else if(strcmp(curr -> filepath, realpath(filename_local, buf)) == 0) { //파일 이름과 실제 절대경로가 같은경우
                
                local_tid = curr -> tid;
                pthread_cancel(local_tid); //해당 쓰레드를 취소시킨다.
                curr -> prev -> next = curr -> next;
                if(curr -> next == NULL)
                            ;
                else 
                    curr -> next -> prev = curr -> prev;

                
                fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] %s/%s deleted\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_dir, backup_realpath), strrchr(curr -> filepath,'/')+1);
                free(curr); //로그파일에 삭제 기록을 기록후에 해당 리스트를 삭제한다.

                curr = listhead;
                while(curr != NULL) {
                    
                    if(curr -> next == NULL) {
                    break;
                    }

                    curr = curr -> next;
                }
                
                fprintf(log_fp,"[%d%02d%02d %02d%02d%02d] remove is executed\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
                return curr; //삭제완료되었다는 기록을 레코드파일에 기록한다.
            }

            else {

                if (curr -> next == NULL) {
                fprintf(stderr,"%s is not exist in backup list\n", filename_local);
                return curr;
                }

                curr = curr -> next;
                
            }

            

        }

    
    }
    
}

void compare_function(void) {

    struct stat statbuf1;
    struct stat statbuf2;
    char *ptr;
    int i = 0;
    int exist_flag = 0;
    int regular_flag = 0;
    char* command_token[3];

    char filename1[BUFFER_MAX];
    char filename2[BUFFER_MAX];

    ptr = strtok(command," ");
    while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    {
        command_token[i] = ptr;          // 자른 문자열 출력
        i++;  
        ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
    }

    if (i != 3) {
        fprintf(stderr, "Usage : %s FILENAME1 FILENAME2\n", command_token[0]); //인자가 부족하면 에러처리후 프롬프트로 돌아간다.
        return;
    }


    strcpy(filename1, command_token[1]);
    strcpy(filename2, command_token[2]);
    stat(filename1, &statbuf1); 
    stat(filename2, &statbuf2);


    if(access(filename1, F_OK) < 0 && access(filename2, F_OK) < 0) { //파일1과 파일2가 서로 둘다 ㄷ존재하는지 확인한다
        fprintf(stderr, "\"%s\" does not exist\n", filename1);
        fprintf(stderr, "\"%s\" does not exist\n", filename2);
        exist_flag = 1;
    }

    else if(access(filename1, F_OK) < 0) { //파일1이 존재하지 않으면 에러처리후 프롬프트로 돌아간다
        fprintf(stderr, "\"%s\" does not exist\n", filename1);
        exist_flag = 1;
    }

    else if(access(filename2, F_OK) < 0) { //파일2가 존재하지 않으면 에러처리후 프롬프트로 돌아간다.
        fprintf(stderr, "\"%s\" does not exist\n", filename2);
        exist_flag = 1;
    }

    if(exist_flag == 1)
        return;


    if(!S_ISREG(statbuf1.st_mode) && !S_ISREG(statbuf2.st_mode)) {
        fprintf(stderr, "\"%s\" is not regular file\n", filename1); //파일1이 일반파일이 아니면 프롬프트로 돌아감
        fprintf(stderr, "\"%s\" is not regular file\n", filename2); //파일2가 일반파일이 아니면 프롬프트로 돌아감
        regular_flag = 1;
    }

    else if(!S_ISREG(statbuf1.st_mode)) {
        fprintf(stderr, "\"%s\" is not regular file\n", filename1); //파일1이 일반파일이 아니면 프롬프트로 돌아감
        regular_flag = 1;
    }

    else if(!S_ISREG(statbuf2.st_mode)) {
        fprintf(stderr, "\"%s\" is not regular file\n", filename2); //파일2가 일반파일이 아니면 프롬프트로 돌아감
        regular_flag = 1;
    }

    if(regular_flag == 1)
        return;


    if((statbuf1.st_mtime == statbuf2.st_mtime) && (statbuf1.st_size == statbuf2.st_size)) {
        printf("\"%s\" and \"%s\" is same!\n", filename1, filename2); //만약 두 파일이 같으면 똑같은 파일이란 메세지를 출력하고 프롬프트로 돌아감
        return;
    }

    else {
        printf("FILE1 : \"%s\" MTIME : %lu SIZE : %lu\n", filename1, statbuf1.st_mtime, statbuf1.st_size); //파일1에 대한 정보 표시
        printf("FILE2 : \"%s\" MTIME : %lu SIZE : %lu\n", filename2, statbuf2.st_mtime, statbuf2.st_size); //파일2에 대한 정보 표시
        return;
    }

}

Backup_list* recover_function(Backup_list* head) {

    char* tmp_filename = "tmp.txt";
    FILE* tmp_fp;
    tmp_fp = fopen(tmp_filename, "w+");
    Backup_list* curr;

    struct tm *tm_p; 
    struct tm time_struct;
    time_t now;
    pthread_t local_tid;

    struct stat tmp_statbuf;
    struct stat backup_statbuf;

    int i = 0;
    int j = 0;
    int select_num;
    char* strrchr_ptr;
    char* ptr;
    char* strtok_ptr;
    char* command_token[4];

    char system_command[BUFFER_MAX];
    char filename_local[BUFFER_MAX];
    char backdir_local[BUFFER_MAX];
    char filename_fullpath[BUFFER_MAX];
    char backdir_fullpath[BUFFER_MAX];

    char filename_final[BUFFER_MAX];
    char final_realpath[BUFFER_MAX];
    char newfile_fullpath[BUFFER_MAX];
   
    
   
    ptr = strtok(command," "); //공백에 따라 커맨드 구분을 시켜준다.
    while(ptr != NULL) {
        command_token[i] = ptr;
        i++;
        ptr = strtok(NULL, " ");
    }
    

    if(access(command_token[1], F_OK) < 0) {
        fprintf(stderr, "\"%s\" does not exist\n", command_token[1]); //해당 파일이 없으면 에러처리후 프롬프트로 돌아감
        fclose(tmp_fp);
        system("rm -rf tmp.txt");
        curr = head;
            while(curr != NULL) { //리스트를 가리키는 포인터가 맨 마지막 리스트를 가리킬수 있도록 설정한다.

                if(curr -> next == NULL)
                break;
                
                curr = curr -> next;
            }
        return curr;
    }
   
    realpath(command_token[1], filename_fullpath); //해당 파일의 절대 경로를 구한다
    strcpy(filename_local, command_token[1]);
    strcpy(backdir_local, backup_dir);
    
    strrchr_ptr = strrchr(filename_local, '/'); //절대경로에서 파일이름만을 추출한다.

    if (strrchr_ptr != NULL)
        strcpy(filename_final, strrchr_ptr+1); //절대경로일 경우의 파일이름 추출방식
    else
        strcpy(filename_final, filename_local); //상대경로일 경우의 파일이름 추출방식

    sprintf(system_command, "%s %s/%s* > %s", "ls", realpath(backdir_local, backdir_fullpath), filename_final, "tmp.txt");
    system(system_command); //파일 리스트들을 모두 출력한다
    
    stat(tmp_filename, &tmp_statbuf);
    
    
    char tmp_buf[tmp_statbuf.st_size];
    char tmp_buf_final[tmp_statbuf.st_size];
    

    fseek(tmp_fp, 0 , SEEK_SET);
    fread(&tmp_buf, tmp_statbuf.st_size, 1, tmp_fp); // 임시파일에 기록된 모든 모든 리스트들을 tmp_buf에 넣는다
    strcpy(tmp_buf_final, tmp_buf); 
    
    
    
    recover_flag = 1;
    strtok_ptr = strtok(tmp_buf,"\n"); //개행문자에 따라 리스트드을 분류한다.
   
    
    printf("0  exit\n");
    while (strtok_ptr != NULL) {
        
        if(strstr(strtok_ptr, "/home") == NULL)
            break;

        stat(strtok_ptr, &backup_statbuf);
        printf("%d.  %s  %lubytes\n", j+1 ,strtok_ptr + strlen(backdir_fullpath) + strlen(filename_final) + 2, backup_statbuf.st_size);
        j++;
        strtok_ptr = strtok(NULL, "\n");
    }
    printf("Choose file to recover : "); //나온 리스트들 중 원하는 리스트를 골라내야한다.
    scanf("%d", &select_num);
    fflush(stdin);
    
    
   
    if(select_num == 0) { 
         fclose(tmp_fp);
         system("rm -rf tmp.txt"); //0번이면 선택을 끝내고 프롬프트로 돌아간다
         curr = head;
            while(curr != NULL) { //리스트를 가리키는 포인터가 맨 마지막 리스트를 가리킬수 있도록 설정한다.

                if(curr -> next == NULL)
                break;
                
                curr = curr -> next;
            }
            return curr;   
    }

    else if(select_num > j || select_num < 0) { //0보다 크거나 선택할수있는 최대번호를 넘어선 경우 끝내고 프롬프트로 돌아간다.
        fprintf(stderr, "Wrong Select\n");
        fclose(tmp_fp);
        system("rm -rf tmp.txt");
        curr = head;
            while(curr != NULL) { //리스트를 가리키는 포인터가 맨 마지막 리스트를 가리킬수 있도록 설정한다.

                if(curr -> next == NULL)
                break;
                
                curr = curr -> next;
            }
            return curr;    
    }

    strtok_ptr = strtok(tmp_buf_final,"\n");

    if(i == 2){
       
        for(int count = 1 ; count <= select_num-1 ; count++) {
            strtok_ptr = strtok(NULL,"\n");
        }
        
        sprintf(system_command, "%s %s %s", "cp", strtok_ptr , filename_fullpath); 
        system(system_command); //원하는 리스트를 골라 해당 파일을 원본파일에 적용시킨다.
        printf("Recovery Success\n");
    }
  
    else if (strcmp(command_token[2], "-n") == 0) {
    
        for(int count = 1 ; count <= select_num-1 ; count++) {
            strtok_ptr = strtok(NULL,"\n");
        }
        
        sprintf(system_command, "%s %s %s", "cp", strtok_ptr , command_token[3]); //원하는 리스트를 골라 해당 파일을 원본파일에 적용시킨다/
        
        if(access(realpath(command_token[3],newfile_fullpath), F_OK) == 0) {
            fprintf(stderr, "\"%s\" is already exist\n", command_token[3]); //만약 이미 존재하는 파일이면 종료시키고 프롬프트로 돌아간다.
            fclose(tmp_fp);
            system("rm -rf tmp.txt");
            curr = head;
            while(curr != NULL) { //리스트를 가리키는 포인터가 맨 마지막 리스트를 가리킬수 있도록 설정한다.

                if(curr -> next == NULL)
                break;
                
                curr = curr -> next;
            }
            return curr;
        }

        system(system_command); //커맨드를 실행시킨다.
        printf("Recovery Success\n");
    }

    
    curr = head -> next;
    while (curr != NULL) {
        time(&now);
        tm_p = localtime_r(&now, &time_struct);
        
        if(strcmp(curr -> filepath, filename_fullpath) == 0 ){ //해당파일에 대한 절대경로를 찾는다.
            local_tid = curr -> tid;
            pthread_cancel(local_tid); //해당 리스트의 쓰레드를 캔슬시킨다.

            curr -> prev -> next = curr -> next;
            if(curr -> next == NULL)
                curr -> next = NULL;
            else 
                curr -> next -> prev = curr -> prev;

            fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] %s/%s deleted\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, backdir_fullpath, strrchr(curr -> filepath,'/')+1);
            free(curr); //삭제되었다는 레코드를 로그파일에 기록하고 해당리스트를 삭제한다.
            break;
        }

        else
        curr = curr -> next;
    }

    curr = head;
    while(curr != NULL) { //리스트를 가리키는 포인터가 맨 마지막 리스트를 가리킬수 있도록 설정한다.

        if(curr -> next == NULL)
            break;

        
        curr = curr -> next;
    }

   
    fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] recover is executed\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    fclose(tmp_fp); //recover가 완료되었다면 레코드를 로그파일에 기록하고 해당 로그파일을 닫는다.
    system("rm -rf tmp.txt"); //임시파일은 다시 삭제시킨다.
    return curr;
    
}
