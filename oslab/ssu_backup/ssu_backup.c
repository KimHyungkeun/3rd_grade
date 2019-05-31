#include "ssu_backup.h"

int main(int argc, char* argv[]) {

   
   char mkdir_command[BUFFER_MAX];
   char logmake_command[BUFFER_MAX];

    strcpy(logfile, "log.txt");

    if(argc > 2) {
         fprintf(stderr, "Usage : %s [directory path]\n", argv[0]);
          exit(1);
    }

    if(argv[1] == NULL) {
         strcpy(backup_dir, "ssu_backup_dir");
         

         if(access(backup_dir, F_OK) == 0) 
                        ;
         

         else {
         sprintf(mkdir_command, "%s %s", "mkdir", backup_dir);
         system(mkdir_command);
         }
            
    }

    else{
           strcpy(backup_dir, argv[1]);
           sprintf(mkdir_command, "%s %s", "mkdir", backup_dir);
           system(mkdir_command);
    }

    
    stat(backup_dir, &statbuf);

    if(access(backup_dir, F_OK) < 0) {
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

void list_function(Backup_list* head) {

    Backup_list* listcurr;
    listcurr = head -> next;

    if(head -> next == NULL)
        return;

    while(listcurr != NULL) {
        printf("%s     %d\n", listcurr -> filepath, listcurr -> period);
        listcurr = listcurr -> next;
    }
    
}

void prompt_environment(void) {

    int idx = 0;
    struct tm *tm_p;
    time_t now;
    Backup_list *head;
    Backup_list *curr;
    char *prompt = "20142332>";
    char backdir_fullpath[BUFFER_MAX];
    
    realpath(backup_dir, backdir_fullpath);
    strcat(backdir_fullpath,"/");
    strcat(backdir_fullpath,logfile);
 
   
    if((log_fp = fopen(backdir_fullpath, "w")) == NULL) {
        fprintf(stderr, "fopen error\n");
        return;
    }
    setbuf(log_fp, NULL);
   

    head = (Backup_list*)malloc(sizeof(Backup_list));
    curr = head;
    curr -> next = NULL;
    curr -> prev = NULL;

    while(1) {

        time(&now);
        tm_p = localtime(&now);

        if(recover_flag == 0)
            fputs(prompt, stdout);
        else {
            recover_flag = 0;
        }
        
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = 0;
        strcpy(command_tmp, command);

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
                fprintf(stderr, "Usage : %s FILENAME [PERIOD] [OPTION]\n", command);
                continue;
            }
            if(add_command_analyzer(head) != 0) 
                continue;
           
            curr -> next = (Backup_list*)malloc(sizeof(Backup_list));
            
            if(pthread_create(&curr -> next -> tid, NULL ,add_function, (void *)curr) != 0) {
                fprintf(stderr, "thread_create error\n");
                continue;
            }
            
            curr = curr -> next;
            continue;
        }

        else if (strstr(command,"remove") != NULL) { //remove 명령
             
             if(strcmp(command,"remove") == 0) {
                fprintf(stderr, "Usage : %s FILENAME [OPTION]\n", command);
                continue;
             }

             curr = remove_function(head);
             continue;
        }

        else if (strstr(command,"compare") != NULL) { //compare 명령
             
             if(strcmp(command,"compare") == 0) {
                 fprintf(stderr, "Usage : %s FILENAME1 FILENAME2\n", command);
                 continue;
             }
             compare_function();
             continue;
        }

        else if (strstr(command,"recover") != NULL) { //recover 명령
             
             if(strcmp(command,"recover") == 0) {
                 fprintf(stderr, "Usage : %s FILENAME [OPTION]\n", command);
                 continue;
             }
             
             recover_function(head, curr);
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

        else if (strcmp(command,"\0") == 0) {
            continue;
        }

        else {
            printf("Unknown Command\n");  //프롬프트 종료 명령
            continue;
        }

        

    }

    fclose(log_fp);

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

    strcpy(filename, command_token[1]);
    stat(filename, &statbuf);
    if(i < 3) {
        fprintf(stderr,"Usage : %s FILENAME [PERIOD] [OPTION]\n",command_token[0]);
        return 1;
    }
 
    if(access(filename, F_OK) < 0) {
        fprintf(stderr,"\"%s\" does not exist\n", filename);
        return 1;
    }

    if (!S_ISREG(statbuf.st_mode)) {
        fprintf(stderr,"Not regular file\n");
        return 1;
    }

    curr = head;
    while (curr != NULL) {
        if(strcmp(curr -> filepath, realpath(filename,filename_buf)) == 0 ) {
            fprintf(stderr,"\"%s\" is already exist in backup list.\n", filename);
            return 1;
        }
        curr = curr -> next;
    }

    strcpy(period, command_token[2]);
  
    

    if (strstr(period,".") != NULL) {
        fprintf(stderr,"%s is not Integer. Try again\n", period);
        return 1;
    }


    if(atoi(period) < 5 || atoi(period) > 10) {
        fprintf(stderr,"Please retry (5 <= PERIOD <= 10)\n");
        return 1;
    }

    time(&now);
    tm_p = localtime(&now);
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

    
    strcpy(command_local, command_tmp);

    ptr = strtok(command_local, " ");
    while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    {
        command_token[i] = ptr;          // 자른 문자열 출력
        i++;  
        
        ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
    }

    strcpy(period, command_token[2]);
    strcpy(filename_local, filename);
    strcpy(backup_local, backup_dir);
    
    listcurr -> next -> prev = listcurr;
    listcurr -> next -> pid = getpid();
    strcpy(listcurr -> next ->  filepath, realpath(filename_local,buf));
    listcurr -> next -> period = atoi(period);
    
    listcurr -> next -> next = NULL;

    strrchr_ptr = strrchr(filename_local, '/');

    if (strrchr_ptr != NULL)
        strcpy(filename_final, strrchr_ptr+1);
    else
        strcpy(filename_final, filename_local);
    
    time(&now);
    tm_p = localtime_r(&now, &time_struct);
    
    sprintf(bck_buf, "%s_%d%02d%02d%02d%02d%02d", filename_final,tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    sprintf(log_buf,"[%d%02d%02d %02d%02d%02d] %s/%s_%d%02d%02d%02d%02d%02d added\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_local, backup_realpath), filename_final,tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    sprintf(cp_command, "%s %s %s/%s","cp",realpath(filename_local,buf),backup_dir,bck_buf);
   
    fprintf(log_fp, "%s", log_buf);
    system(cp_command);
    sleep(atoi(period));
  
    while(1) {
        time(&now);
        tm_p = localtime_r(&now, &time_struct);

        sprintf(bck_buf, "%s_%d%02d%02d%02d%02d%02d", filename_final ,tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
        sprintf(log_buf,"[%d%02d%02d %02d%02d%02d] %s/%s_%d%02d%02d%02d%02d%02d generated\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_local, backup_realpath), filename_final, tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
        sprintf(cp_command, "%s %s %s/%s","cp",realpath(filename_local,buf),backup_dir,bck_buf);
        
        fprintf(log_fp, "%s", log_buf);
        system(cp_command);
        sleep(atoi(period));
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
    strrchr_ptr = strrchr(filename_local, '/');

    if (strrchr_ptr != NULL)
        strcpy(filename_final, strrchr_ptr+1);
    else
        strcpy(filename_final, filename_local);

    

    if(strcmp(command_token[1], "-a") == 0) {
        curr = listhead -> next;
        while (curr != NULL) { 
            time(&now);
            tm_p = localtime_r(&now, &time_struct);
            local_tid = curr -> tid;
            pthread_cancel(local_tid);
     
            curr -> prev -> next = curr -> next;
            if(curr -> next == NULL)
                curr -> next = NULL;
            else 
                curr -> next -> prev = curr -> prev;

            fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] %s/%s deleted\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_local, backup_realpath) ,strrchr(curr -> filepath, '/')+1);
            free(curr);
            curr = listhead -> next;
        }
        curr = listhead;
        fprintf(log_fp,"[%d%02d%02d %02d%02d%02d] remove is executed\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
        return curr;
    }

    else {
        
        curr = listhead -> next;
        while (curr != NULL) {
            time(&now);
            tm_p = localtime_r(&now, &time_struct);

            if(strcmp(curr -> filepath, realpath(filename_local, buf)) == 0) {
                
                local_tid = curr -> tid;
                pthread_cancel(local_tid);
                curr -> prev -> next = curr -> next;
                if(curr -> next == NULL)
                            ;
                else 
                    curr -> next -> prev = curr -> prev;

                
                fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] %s/%s deleted\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, realpath(backup_dir, backup_realpath), strrchr(curr -> filepath,'/')+1);
                free(curr);

                curr = listhead -> next;
                while(curr != NULL) {
                    
                    if(curr -> next == NULL) {
                    break;
                    }

                    curr = curr -> next;
                }
                
                fprintf(log_fp,"[%d%02d%02d %02d%02d%02d] remove is executed\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
                return curr;
            }

            else
                curr = curr -> next;
        }

    fprintf(log_fp,"[%d%02d%02d %02d%02d%02d] remove is executed\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    return curr;
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
        fprintf(stderr, "Usage : %s FILENAME1 FILENAME2\n", command_token[0]);
        return;
    }


    strcpy(filename1, command_token[1]);
    strcpy(filename2, command_token[2]);
    stat(filename1, &statbuf1);
    stat(filename2, &statbuf2);


    if(access(filename1, F_OK) < 0 && access(filename2, F_OK) < 0) {
        fprintf(stderr, "\"%s\" does not exist\n", filename1);
        fprintf(stderr, "\"%s\" does not exist\n", filename2);
        exist_flag = 1;
    }

    else if(access(filename1, F_OK) < 0) {
        fprintf(stderr, "\"%s\" does not exist\n", filename1);
        exist_flag = 1;
    }

    else if(access(filename2, F_OK) < 0) {
        fprintf(stderr, "\"%s\" does not exist\n", filename2);
        exist_flag = 1;
    }

    if(exist_flag == 1)
        return;


    if(!S_ISREG(statbuf1.st_mode) && !S_ISREG(statbuf2.st_mode)) {
        fprintf(stderr, "\"%s\" is not regular file\n", filename1);
        fprintf(stderr, "\"%s\" is not regular file\n", filename2);
        regular_flag = 1;
    }

    else if(!S_ISREG(statbuf1.st_mode)) {
        fprintf(stderr, "\"%s\" is not regular file\n", filename1);
        regular_flag = 1;
    }

    else if(!S_ISREG(statbuf2.st_mode)) {
        fprintf(stderr, "\"%s\" is not regular file\n", filename2);
        regular_flag = 1;
    }

    if(regular_flag == 1)
        return;


    if((statbuf1.st_mtime == statbuf2.st_mtime) && (statbuf1.st_size == statbuf2.st_size)) {
        printf("\"%s\" and \"%s\" is same!\n", filename1, filename2);
        return;
    }

    else {
        printf("FILE1 : \"%s\" MTIME : %lu SIZE : %lu\n", filename1, statbuf1.st_mtime, statbuf1.st_size);
        printf("FILE2 : \"%s\" MTIME : %lu SIZE : %lu\n", filename2, statbuf2.st_mtime, statbuf2.st_size);
        return;
    }

}

void recover_function(Backup_list* head, Backup_list* curr) {

    char* tmp_filename = "tmp.txt";
    FILE* tmp_fp;
    tmp_fp = fopen(tmp_filename, "w+");

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
   
    
   
    ptr = strtok(command," ");
    while(ptr != NULL) {
        command_token[i] = ptr;
        i++;
        ptr = strtok(NULL, " ");
    }
    

    if(access(command_token[1], F_OK) < 0) {
        fprintf(stderr, "\"%s\" does not exist\n", command_token[1]);
        fclose(tmp_fp);
        system("rm -rf tmp.txt");
        return ;
    }
   
    realpath(command_token[1], filename_fullpath);
    strcpy(filename_local, command_token[1]);
    strcpy(backdir_local, backup_dir);
    
    strrchr_ptr = strrchr(filename_local, '/');

    if (strrchr_ptr != NULL)
        strcpy(filename_final, strrchr_ptr+1);
    else
        strcpy(filename_final, filename_local);

    sprintf(system_command, "%s %s/%s* > %s", "ls", realpath(backdir_local, backdir_fullpath), filename_final, "tmp.txt");
    system(system_command);
    
    stat(tmp_filename, &tmp_statbuf);
    
    
    char tmp_buf[tmp_statbuf.st_size];
    char tmp_buf_final[tmp_statbuf.st_size];

    fread(&tmp_buf, sizeof(tmp_buf), 1, tmp_fp);
    strcpy(tmp_buf_final, tmp_buf);
    
    
    recover_flag = 1;
    strtok_ptr = strtok(tmp_buf,"\n");
   
    
    printf("0  exit\n");
    while (strtok_ptr != NULL) {
        
        if(strstr(strtok_ptr, "/home") == NULL)
            break;

        stat(strtok_ptr, &backup_statbuf);
        printf("%d.  %s  %lubytes\n", j+1 ,strtok_ptr + strlen(backdir_fullpath) + strlen(filename_final) + 2, backup_statbuf.st_size);
        j++;
        strtok_ptr = strtok(NULL, "\n");
    }
    printf("Choose file to recover : ");
    scanf("%d", &select_num);
    fflush(stdin);
    
    
   
    if(select_num == 0) {
         fclose(tmp_fp);
         system("rm -rf tmp.txt");
         return;    
    }

    else if(select_num > j || select_num < 0) {
        fprintf(stderr, "Wrong Select\n");
        fclose(tmp_fp);
        system("rm -rf tmp.txt");
        return;    
    }

    strtok_ptr = strtok(tmp_buf_final,"\n");

    if(i == 2){
       
        for(int count = 1 ; count <= select_num-1 ; count++) {
            strtok_ptr = strtok(NULL,"\n");
        }
        
        sprintf(system_command, "%s %s %s", "cp", strtok_ptr , filename_fullpath);
        system(system_command);
        printf("Recovery Success\n");
    }
  
    else if (strcmp(command_token[2], "-n") == 0) {
    
        for(int count = 1 ; count <= select_num-1 ; count++) {
            strtok_ptr = strtok(NULL,"\n");
        }
        
        sprintf(system_command, "%s %s %s", "cp", strtok_ptr , command_token[3]);
        
        if(access(realpath(command_token[3],newfile_fullpath), F_OK) == 0) {
            fprintf(stderr, "\"%s\" is already exist\n", command_token[3]);
            fclose(tmp_fp);
            system("rm -rf tmp.txt");
            return;
        }

        system(system_command);
        printf("Recovery Success\n");
    }

    
    curr = head -> next;
    while (curr != NULL) {
        time(&now);
        tm_p = localtime_r(&now, &time_struct);
        
        if(strcmp(curr -> filepath, filename_fullpath) == 0 ){
            local_tid = curr -> tid;
            pthread_cancel(local_tid);

            curr -> prev -> next = curr -> next;
            if(curr -> next == NULL)
                curr -> next = NULL;
            else 
                curr -> next -> prev = curr -> prev;

            fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] %s/%s deleted\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec, backdir_fullpath, strrchr(curr -> filepath,'/')+1);
            free(curr);
            break;
        }

        else
        curr = curr -> next;
    }

    curr = head -> next;
    while(curr != NULL) {

        if(curr -> next == NULL)
            break;

        else
            curr = curr -> next;
    }

   
    fprintf(log_fp, "[%d%02d%02d %02d%02d%02d] recover is executed\n",tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    fclose(tmp_fp);
    system("rm -rf tmp.txt");
    return;
    
}
