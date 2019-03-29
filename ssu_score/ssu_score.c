#include "ssu_score.h"
#include "ssu_time.h"

  
int main(int argc, char *argv[])
{
   int opt;
   int qname_flag = 0;
   int error_flag = 0;
   int print_flag = 0;	
   char directory_path_ans[20] = "\0"; //ANS_DIR의 경로	   
   char directory_path_std[20] = "\0"; //STD_DIR의 경로
   gettimeofday(&begin_t, NULL);

   opt = getopt(argc, argv, "ept");
   switch(opt) {
       case 't' :
        qname_flag = 1;
        break;

       case 'e' :
        error_flag = 1;
        break;

       case 'p' :
        print_flag = 1;
        break;
   }

   strcpy(directory_path_std, argv[1]);
   strcat(directory_path_std, "/");
   strcpy(directory_path_ans, argv[2]); 
   strcat(directory_path_ans, "/");

   if(argc < 3) {
	fprintf(stderr,"Usage : %s STD_DIR ANS_DIR\n",argv[0]);
	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(1);
	}
	
	//blank_problem_check(directory_path_std, directory_path_ans); //빈칸 채우기 문제 	
    ssu_score_table_create(directory_path_ans);
	//program_autocompile(directory_path_std, directory_path_ans, qname_flag); //program auto complile
    //program_problem_check(directory_path_std, directory_path_ans); //program problem compare
    score_table_create(directory_path_std, directory_path_ans, print_flag);
	

	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void program_autocompile(char* directory_path_std, char* directory_path_ans, int flag){
    struct  dirent **namelist;
    int     count, except = 0;
    int     idx;
    int     stdoutfile_fd, error_fd, fd_backup, fd_backup2;
    int not_c;
    char exe_filename[30] = "\0";
    char stdout_filename[30] = "\0";
    char txt_filename[30] = "\0";
    char exe_syntax[30] = "\0";
    char gcc_syntax[30] = "\0";
    char run_syntax[30] = "\0";
    char stdoutfile_name[30] = "\0";
    char errorfile_name[30] = "\0";
    char killall_command[30] = "\0";
    int compile_count = 0;
    
    // ANS_DIR auto compile part
    if((count = scandir(directory_path_ans, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_ans, strerror(errno));
        exit(1);
    }

    /*for(idx = 0; idx < count; idx++) {
        if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
            continue;
        
        if(strcmp(namelist[idx] -> d_name, "score_table.csv") == 0 || strpbrk(namelist[idx] -> d_name,"-") != NULL)
            continue;

        printf("%s\n", namelist[idx] -> d_name);
    }*/

    
    for(idx = 0; idx < count; idx++) {
        if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
            continue;
        
        if(strcmp(namelist[idx] -> d_name, "score_table.csv") == 0 || strpbrk(namelist[idx] -> d_name,"-") != NULL)
            continue;

        else {
        strcpy(dir_path_backup_ans, directory_path_ans);
        strcat(directory_path_ans, namelist[idx] -> d_name);
        strcat(directory_path_ans, "/");
        strcpy(sub_path_ans_backup, directory_path_ans); // ANS_DIR/SUBDIR

        sub_ans = opendir(directory_path_ans);
        while( entry_sub_ans = readdir(sub_ans)){
            //compile_count++;
            //printf("complie count : %d\n", compile_count);
            //printf("%s\n", entry_sub_ans -> d_name);
            if(strcmp(entry_sub_ans -> d_name,".") == 0 || strcmp(entry_sub_ans-> d_name,"..") == 0)
            continue;

            sprintf(exe_filename,"%s%s",namelist[idx] -> d_name,".exe");
            if(strcmp(exe_filename,entry_sub_ans -> d_name) == 0)
                continue;
            sprintf(stdout_filename,"%s%s",namelist[idx] -> d_name,".stdout");
            if(strcmp(stdout_filename,entry_sub_ans -> d_name) == 0)
                continue;
            
            else {
            strcat(directory_path_ans, namelist[idx] -> d_name);
    
            sprintf(exe_syntax, "%s%s" , sub_path_ans_backup, namelist[idx] -> d_name);
            printf("exe_syntax %s\n", exe_syntax);

            sprintf(stdoutfile_name, "%s%s", exe_syntax, ".stdout");
            printf("stdoutfile_name %s\n", stdoutfile_name);
            
            
            if( (stdoutfile_fd = open(stdoutfile_name, O_RDWR | O_CREAT | O_TRUNC, 0640)) < 0) {
                    fprintf(stderr, "open error for %s\n", stdoutfile_name);
                    exit(1);
            }
                fd_backup = dup(1);
              
                sprintf(gcc_syntax, "%s %s%s %s %s %s%s", "gcc" , directory_path_ans,".c", "-lpthread","-o", exe_syntax,".exe");
                system(gcc_syntax);

                printf("%s\n", gcc_syntax);
                dup2(stdoutfile_fd, 1);

                
                first = time(NULL);
                sprintf(run_syntax, "%s%s%s", "./",exe_syntax, ".exe");
                system(run_syntax);
                second = time(NULL);

                dup2(fd_backup, 1);
                printf("wait time is %f seconds\n\n", difftime(second, first));
                close(stdoutfile_fd); 

             strcpy(directory_path_ans, sub_path_ans_backup );
            }
            
        }
        closedir(sub_ans); 
        strcpy(directory_path_ans,dir_path_backup_ans);
       
        }
    }
   
    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    free(namelist);

    // STD_DIR auto compile part
    
    if((count = scandir(directory_path_std, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_std, strerror(errno));
        exit(1);
    }

    for(idx = 0; idx < count; idx++) {
        if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
            continue;

        else {
        strcpy(dir_path_backup_std, directory_path_std);
        strcat(directory_path_std, namelist[idx] -> d_name);
        strcat(directory_path_std, "/");
        strcpy(sub_path_std_backup, directory_path_std); // STD_DIR/SUBDIR

        sub_std = opendir(directory_path_std);
        while( entry_sub_std = readdir(sub_std)){
            //printf("%s\n",entry_sub_std -> d_name);
            //compile_count++;
            //printf("complie count : %d\n", compile_count);
            //printf("%s\n", entry_sub_ans -> d_name);
            if(strcmp(entry_sub_std -> d_name,".") == 0 || strcmp(entry_sub_std-> d_name,"..") == 0)
            continue;

            /*sprintf(exe_filename,"%s%s",namelist[idx] -> d_name,".exe");
            if(strcmp(exe_filename,entry_sub_std -> d_name) == 0)
                continue;
            sprintf(stdout_filename,"%s%s",namelist[idx] -> d_name,".stdout");
            if(strcmp(stdout_filename,entry_sub_std -> d_name) == 0)
                continue;*/
        
            if(strpbrk(entry_sub_std -> d_name, "-") != NULL)
                continue;

            for(int i=0 ; i < strlen(entry_sub_std -> d_name) ; i++) {
                
                if(entry_sub_std -> d_name [i] == 'e' || entry_sub_std -> d_name [i]== 's') {
                    not_c = 1;
                }

            }
            
            if(not_c == 1) {
                not_c = 0;
                continue;
            } 
                

            else {
            strcat(directory_path_std, entry_sub_std -> d_name); //STD_DIR/SUBDIR/*.txt or .c
            
            for(int i=0;i<strlen(directory_path_std);i++)
            {
                    if(directory_path_std[i] == 'c')
                    directory_path_std[i] = '\0';
            }

            for(int i=0;i<strlen(directory_path_std);i++)
            {
                    if(directory_path_std[i] == '.')
                    directory_path_std[i] = '\0';
            }

          
            printf("directory_path_std : %s\n", directory_path_std);
            sprintf(exe_syntax, "%s" , directory_path_std);
            printf("exe_syntax %s\n", exe_syntax);

            sprintf(stdoutfile_name, "%s%s", exe_syntax, ".stdout");
            printf("stdoutfile_name %s\n", stdoutfile_name);

            sprintf(errorfile_name, "%s%s", exe_syntax, ".error");
            printf("errorfile_name %s\n", errorfile_name);
            
            
            if( (stdoutfile_fd = open(stdoutfile_name, O_RDWR | O_CREAT | O_TRUNC, 0640)) < 0 ) {
                    fprintf(stderr, "open error for %s\n", stdoutfile_name);
                    exit(1);
            }

            if( (error_fd = open(errorfile_name, O_RDWR | O_CREAT | O_TRUNC, 0640)) < 0 ) {
                    fprintf(stderr, "open error for %s\n", errorfile_name);
                    exit(1);
            }

                fd_backup = dup(1);
                fd_backup2 = dup(2);
                dup2(stdoutfile_fd, 1);
                dup2(error_fd,2);

                if(flag == 1)
                    sprintf(gcc_syntax, "%s %s%s %s %s %s%s", "gcc" , directory_path_std,".c", "-lpthread","-o", exe_syntax,".exe");
                else 
                    sprintf(gcc_syntax, "%s %s%s %s %s%s", "gcc" , directory_path_std,".c", "-o", exe_syntax,".exe");

                system(gcc_syntax);

                
                sprintf(killall_command,"%s %s%s", "killall", exe_syntax,"*");
                sprintf(run_syntax, "%s%s%s %s", "./",exe_syntax, ".exe","&");


                first = time(NULL);
                system(run_syntax);
                second = time(NULL);
                
                
                dup2(fd_backup, 1);
                dup2(fd_backup2,2);
                printf("wait time is %f seconds\n\n", difftime(second, first));
                close(stdoutfile_fd); 
                close(error_fd);

             strcpy(directory_path_std, sub_path_std_backup );
            }
            
        }
        closedir(sub_std); 
        strcpy(directory_path_std,dir_path_backup_std);
       
        }
    }
        

    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    free(namelist);

  
    system("ps -ef | grep STD_DIR/");
    printf("Please wait 10 seconds...\n");
    sleep(10);
    printf("kill process when proceed time limits 5 sec or stop\n");
    system("killall 12.exe");
    system("killall 13.exe");
    system("ps -ef | grep STD_DIR/");
    
    
}

void program_problem_check(char* directory_path_std, char* directory_path_ans) {
    struct  dirent **namelist;
    int ans_count, std_count ,except = 0;
    int ans_idx, std_idx;
    int ans_length, std_length;
    int ans_fd, std_fd;
    int ans_not_c, std_not_c;
    char exe_filename[30] = "\0";
    char stdout_filename[30] = "\0";
    char txt_filename[30] = "\0";
    char exe_syntax[30] = "\0";
    char gcc_syntax[30] = "\0";
    char run_syntax[30] = "\0";
    char stdoutfile_name[30] = "\0";
    int compile_count = 0;

    if((ans_count = scandir(directory_path_ans, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_ans, strerror(errno));
        exit(1);
        } 
   
    for(ans_idx = 0; ans_idx < ans_count; ans_idx++) {
        if(strcmp(namelist[ans_idx] -> d_name,".") == 0 || strcmp(namelist[ans_idx] -> d_name,"..") == 0)
        continue;

        if(strcmp(namelist[ans_idx] -> d_name,"score_table.csv") == 0 || strpbrk(namelist[ans_idx] -> d_name, "-") != NULL) 
        continue;
        
        else {
        strcpy(dir_path_backup_ans, directory_path_ans);
        strcat(directory_path_ans, namelist[ans_idx] -> d_name);
        strcat(directory_path_ans, "/"); // ANS_DIR/SUBDIR
        strcpy(sub_path_ans_backup, directory_path_ans); // ANS_DIR/SUBDIR

        sub_ans = opendir(directory_path_ans);
        while ( entry_sub_ans = readdir(sub_ans) ) {
            strcat(directory_path_ans, entry_sub_ans -> d_name); 
            for(int i = 0; i < strlen(entry_sub_ans -> d_name); i++){
                    if( entry_sub_ans -> d_name[i] == 's' ) {
                        ans_not_c = 1;
                        break;
                    }

                    else
                        ans_not_c = 0;
            }

            if(ans_not_c == 1) {

               if((fd_ans = open(directory_path_ans, O_RDONLY)) < 0 ) {
                   fprintf(stderr, "open error for %s\n", directory_path_ans);
                   exit(1);
               }
               ans_length = read(fd_ans, buf_ANS, BUFFER_SIZE);
        }

            closedir(sub_ans);
            strcpy(directory_path_ans,dir_path_backup_ans);
        }
    }
    
        for(ans_idx = 0; ans_idx < ans_count; ans_idx++) {
        free(namelist[ans_idx]);
        }

    }
        free(namelist);
}

void blank_problem_check(char* directory_path_std, char* directory_path_ans) {
        
}

void ssu_score_table_create(char* directory_path_ans) {

	
    struct  dirent **namelist;
    int select_mode;
 	int fd_score;//fd : 파일 디스크립터, count : 읽은 buf의 갯수
    int fd_tmp;
    int idx, nameptr_count;
    int fd_backup, fd_backup2;
    int except_dot=0, except_csv=0, except_sum = 0;
    double txt_score, c_score;
    char *ptr, *name_ptr;
	
   /*if((count = scandir(directory_path_ans, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_ans, strerror(errno));
        exit(1);
        }*/ 
   
    if((fd_score = open("ANS_DIR/score_table.csv", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n","score_table.csv");
        exit(1);
    }

    if((fd_tmp = open("dir.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n","dir.txt");
        exit(1);
    }
    
    system("ls -vd ANS_DIR/*/ > dir.txt");
    read(fd_tmp, buf_dirname, BUFFER_SIZE);
    close(fd_tmp);
    system("rm dir.txt");

    ptr = strtok(buf_dirname, "\n");
    while(ptr != NULL) {
        strcpy(ssu_score_tab_for[pro_count].name, ptr);
        //printf("%s\n", ssu_score_tab_for[pro_count].name);
        pro_count++;
        ptr = strtok(NULL,"\n");
    }
    //printf("%d\n", pro_count);

    for(idx = 0 ; idx < pro_count; idx++) {
        name_ptr = strtok(ssu_score_tab_for[idx].name, "/");
        nameptr_count = 0;
            while(name_ptr != NULL) {
            
            if(nameptr_count == 1) {
                strcpy(ssu_score_tab_for[idx].name, name_ptr);
                //printf("%s\n", ssu_score_tab_for[idx].name);
                break;
            }
            nameptr_count ++;
            name_ptr = strtok(NULL,"/");
        }
    }

        
    printf("Select Mode(1 or 2) : ");
    scanf("%d", &select_mode);

    while(1) {
    if(select_mode == 1){

        printf("Input score for txt_file : ");
        scanf("%lf",&txt_score);
        printf("Input score for c_file : ");
        scanf("%lf",&c_score);
        for(idx = 0; idx< pro_count ; idx++) {
           
            
            if(strpbrk(ssu_score_tab_for[idx].name, "-") != NULL)
            ssu_score_tab_for[idx].score = txt_score;

            else
            ssu_score_tab_for[idx].score = c_score;
        
            ssu_score_tab_for[idx].comma = ',';
            ssu_score_tab_for[idx].linejump = '\n';
        }
        break;
    }

    else if(select_mode == 2) {
     for(idx = 0; idx< pro_count ; idx++) {
            printf("Input of ");
	        printf("%s",ssu_score_tab_for[idx].name);
        
            if(strpbrk(ssu_score_tab_for[idx].name, "-") != NULL)
            printf("%s :",".txt");
            else
            printf("%s :",".c");
            
            scanf("%lf",&ssu_score_tab_for[idx].score);
            ssu_score_tab_for[idx].comma = ',';
            ssu_score_tab_for[idx].linejump = '\n';
        }
        break;
    }

        else {
        printf("Select Mode(1 or 2) Retry : ");
        scanf("%d", &select_mode);
        }

    }

       fd_backup = dup(1);
       dup2(fd_score,1);
       for(idx = 0 ; idx < pro_count; idx++) {
           printf("%s%c%.2lf%c", ssu_score_tab_for[idx].name,ssu_score_tab_for[idx].comma,ssu_score_tab_for[idx].score,ssu_score_tab_for[idx].linejump);
       }
       dup2(fd_backup,1);
       close(fd_score);
}

void score_table_create(char *directory_path_std, char *directory_path_ans, int print_flag) {

    struct dirent **namelist;
    int std_count, except_dot = 0;
    int idx, jdx;
    int fd_total, fd_backup, fd_backup2;
    double  total_sum, avg = 0;
    double  print_sum_count[20];
    double  print_sum_count_total = 0;
    char exe_syntax[30] = "\0";
    char gcc_syntax[30] = "\0";
    char run_syntax[30] = "\0";
    char stdoutfile_name[30] = "\0";
    char buf[BUFFER_SIZE];

    

    if((std_count = scandir(directory_path_std, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_std, strerror(errno));
        exit(1);
        } 

    if((fd_total = open("score.csv", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
        fprintf(stderr, "creat error for %s\n","score.csv");
        exit(1);
    }
        

    fd_backup = dup(1);
    dup2(fd_total , 1);

    printf(",");
    for(idx = 0; idx < pro_count; idx++) {
        printf("%s,", ssu_score_tab_for[idx].name);
    }
    printf("Sum\n");
    for(idx = 0; idx < std_count; idx++) {
        if(strcmp(namelist[idx] -> d_name, ".") == 0 || strcmp(namelist[idx] -> d_name, "..") == 0) {
            except_dot++ ;
            continue;
        }

        total_sum = 0;
        strcpy(total_score_tab_for[idx].student, namelist[idx] -> d_name);
        printf("%s,",total_score_tab_for[idx].student);

        for(jdx = 0; jdx < pro_count ; jdx++){
        total_score_tab_for[jdx].score = ssu_score_tab_for[jdx].score;
        printf("%.2lf", total_score_tab_for[jdx].score);
        printf(",");
        total_sum += total_score_tab_for[jdx].score;
        }
        print_sum_count[idx] = total_sum;
        printf("%.2lf\n", total_sum);
        
    }

    dup2(fd_backup, 1);
    close(fd_total);


    if(print_flag == 1) {
        for(idx = except_dot ; idx < std_count ;idx++) {
    
        if(strcmp(namelist[idx] -> d_name, ".") == 0 || strcmp(namelist[idx] -> d_name, "..") == 0) {
            continue;
        }   

        printf("%s : %.2lf\n", namelist[idx] -> d_name, print_sum_count[idx]);
        print_sum_count_total += print_sum_count[idx];

        }

        avg = print_sum_count_total / (std_count - except_dot);
        printf("Total Average : %.2lf\n", avg);
    }

}

