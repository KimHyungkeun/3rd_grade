#include "ssu_score.h"
#include "ssu_time.h"

  
int main(int argc, char *argv[])
{
   int opt;
   int qname_flag = 0;
   int error_flag = 0;
   int print_flag = 0;
   int help_flag = 0;
   char t_filename[20]; //filename by using t option
   char e_filename[20];	//filename by using e option
   char directory_path_ans[20] = "\0"; //ANS_DIR의 경로	   
   char directory_path_std[20] = "\0"; //STD_DIR의 경로
   gettimeofday(&begin_t, NULL);

   if(argc < 3) {
	fprintf(stderr,"Usage : %s STD_DIR ANS_DIR [option]\n",argv[0]);
	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(1);
	}

   strcpy(directory_path_std, argv[1]);
   strcat(directory_path_std, "/");
   strcpy(directory_path_ans, argv[2]); 
   strcat(directory_path_ans, "/");

   while ((opt = getopt(argc, argv, "phe:t: ")) != -1) {
    switch(opt) {

        case 'h' :
            help_flag = 1;
            break;
         
        case 't' :
            qname_flag = 1;
            for(int i = 0 ; i < argc ; i++) {
                if(strcmp(argv[i], "-t") == 0) {
                    strcpy(t_filename, argv[i+1]);
                    memcpy(t_filename, optarg, strlen(optarg));
                    break;
                }
            }
            break;
        
        case 'e' :
            error_flag = 1;
            for(int i = 0 ; i < argc ; i++) {
                if(strcmp(argv[i], "-e") == 0) {
                    strcpy(e_filename, argv[i+1]);
                    memcpy(e_filename, optarg, strlen(optarg));
                    break;
                }
            }
            break;

        case 'p' :
            print_flag = 1;
            break;

        case '?':
            if(optopt == 't') {
                printf("option -t requires [QNAME]\n");
                gettimeofday(&end_t,NULL);
                ssu_runtime(&begin_t, &end_t);
	            exit(1);
            }
            else if(optopt == 'e') {
                printf("option -e requires [DIRNAME]\n");
                gettimeofday(&end_t,NULL);
                ssu_runtime(&begin_t, &end_t);
	            exit(1);
            }
    
    }
   }
   printf("%d %d %d %d\n", help_flag, qname_flag, error_flag, print_flag);

   /*for(int i = 0 ; i < argc ; i++) {
                if(strcmp(argv[i], "-t") == 0) {
                    printf("index : %d option : %s\n", i, argv[i]);
                    break;
                }
            }
    for(int i = 0 ; i < argc ; i++) {
                if(strcmp(argv[i], "-e") == 0) {
                    printf("index : %d option : %s\n", i, argv[i]);
                    break;
                }
            }*/

   if(help_flag) {
	fprintf(stderr,"Usage : %s STD_DIR ANS_DIR [option]\n-e <DIRNAME>   print error on 'DIRNAME/ID/qname_error.txt' file\n-t <QNAME>   compile QNAME.c with -lpthread option\n-h  print usage\n-p  print student's score and total average\n",argv[0]);
	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(1);
	}

    

    ssu_score_table_create(directory_path_ans);
    score_table_create_proto(directory_path_std, directory_path_ans); //make prototpye of scoretable
    program_autocompile(directory_path_std, directory_path_ans, qname_flag, error_flag, t_filename, e_filename); //program auto complile
    program_problem_check(directory_path_std, directory_path_ans); //program problem compare
    blank_problem_check(directory_path_std, directory_path_ans); //빈칸 채우기 문제 	
    score_table_create(directory_path_std, directory_path_ans, print_flag);
    

	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void program_autocompile(char* directory_path_std, char* directory_path_ans, int qname_flag, int error_flag, char* t_filename, char* e_filename){
    struct  dirent **namelist, **stdnamelist;
    int     count, except = 0;
    int     std_count=0;
    int     err_dir_count = 0;
    int     idx;
    int     stdoutfile_fd, error_fd, fd_backup, fd_backup2;
    int not_c;
    int ptr_count;
    char root_add_filename[10] = "\0"; 
    char token_divide[30] = "\0";
    char exe_filename[30] = "\0";
    char stdout_filename[30] = "\0";
    char txt_filename[30] = "\0";
    char exe_syntax[30] = "\0";
    char gcc_syntax[30] = "\0";
    char run_syntax[30] = "\0";
    char stdoutfile_name[30] = "\0";
    char errorfile_name[30] = "\0";
    char killall_command[30] = "\0";
    char remove_error_directory[30] = "\0";
    char make_error_directory[30] = "\0";
    char move_error_file_syntax[50] = "\0";
    char *ptr;
    int compile_count = 0;

   
    
    // ANS_DIR auto compile part
    if((count = scandir(directory_path_ans, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_ans, strerror(errno));
        exit(1);
    }
    
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

                //printf("%s\n", gcc_syntax);
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
            
            if(strcmp(entry_sub_std -> d_name,".") == 0 || strcmp(entry_sub_std-> d_name,"..") == 0)
            continue;

        
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

            sprintf(errorfile_name, "%s%s", exe_syntax, "_error.txt");
            printf("errorfile_name %s\n", errorfile_name);

            
            ptr = strstr(directory_path_std,"/12");
            if(ptr == NULL)
                ptr = "null";
            sprintf(root_add_filename, "%s%s", "/", t_filename);
            //printf("strcmp : %d\n", strcmp(ptr, root_add_filename));
            
            
            if( (stdoutfile_fd = open(stdoutfile_name, O_RDWR | O_CREAT | O_TRUNC, 0755)) < 0 ) {
                    fprintf(stderr, "open error for %s\n", stdoutfile_name);
                    exit(1);
            }

            if( (error_fd = open(errorfile_name, O_RDWR | O_CREAT | O_TRUNC, 0755)) < 0 ) {
                    fprintf(stderr, "open error for %s\n", errorfile_name);
                    exit(1);
                 }

                    fd_backup = dup(1);
                    fd_backup2 = dup(2);
                    dup2(stdoutfile_fd, 1);
                    dup2(error_fd,2);
                    

                    if(qname_flag == 1 && strcmp(root_add_filename, ptr) == 0) {
                        sprintf(gcc_syntax, "%s %s%s %s %s %s%s", "gcc" , directory_path_std,".c", "-lpthread","-o", exe_syntax,".exe");
                    }
                    else {
                        sprintf(gcc_syntax, "%s %s%s %s %s%s", "gcc" , directory_path_std,".c", "-o", exe_syntax,".exe");
                    }

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
    printf("Please wait... 10 secs \n");
    sleep(10);
    printf("kill process when proceed time limits 5 sec or stoppd\n");
    system("killall 12.exe");
    system("killall 13.exe");

    
     //error option process
     if(error_flag == 1) {
        sprintf(remove_error_directory,"%s %s","rm -rf",e_filename); // because remove all files and sub directory
        sprintf(make_error_directory,"%s %s","mkdir",e_filename); // because remove all files and sub directory

        if((count = scandir(".", &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", "ssu_score", strerror(errno));
        exit(1);
        }

        for(idx = 0; idx < count; idx++) {
            if(strcmp(namelist[idx] -> d_name, e_filename) == 0 )
                system(remove_error_directory); // because remove all files and sub directory
        }

        if((std_count = scandir("STD_DIR/", &stdnamelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n","STD_DIR", strerror(errno));
        exit(1);
        }

        mkdir(e_filename, 0775);

        for(int i = 2; i < std_count; i++) { // move all error file to "error" directory from 20190001 ~ 20190020
                sprintf(make_error_directory , "%s%s%s", e_filename,"/",stdnamelist[i] -> d_name);
                mkdir(make_error_directory, 0775);
        }

        for(int i = 2; i < std_count; i++) {
                sprintf(directory_path_std, "%s%s%s%s%s", "STD_DIR","/",stdnamelist[i] -> d_name,"/", "*_error.txt");
                sprintf(make_error_directory , "%s%s%s%s", e_filename,"/",stdnamelist[i] -> d_name,"/");
                sprintf(move_error_file_syntax, "%s %s %s", "mv", directory_path_std, make_error_directory);
                //printf("%s\n", move_error_file_syntax);
                system(move_error_file_syntax);
        }

        strcpy(directory_path_std,dir_path_backup_std); //backup STD_DIR/

         for(idx = 0; idx < std_count; idx++) {
            free(stdnamelist[idx]);
        }
            free(stdnamelist);

         for(idx = 0; idx < count; idx++) {
            free(namelist[idx]);
        }
        free(namelist);
    
    }

    
    
}

void program_problem_check(char* directory_path_std, char* directory_path_ans) {
    struct  dirent **namelist;
    int ans_count, std_count;
    int cfile_index = 0;
    int stdout_count;
    int ans_idx, std_idx;
    int ans_length, std_length;
    int ans_fd, std_fd;
    char ls_syntax[50] = "\0"; // ls syntax
    char txt_filename[30] = "\0";
    char ans_filepathbuf[BUFFER_SIZE] = "\0"; //ANS_DIR buf of filepath
    char std_filepathbuf[BUFFER_SIZE] = "\0"; //STD_DIR buf of filepath
    char *stdout_filename[4]; // only *.stdout 
    char *ans_filepathname[4]; // ANS_DIR/*/*.stdout
    char std_filepathname[4][20][30]; // STD_DIR/*/*.stdout
    char *ptr; //location pointer

   

    if((ans_fd = open("ans_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n", "ans_file.txt");
        exit(1);
    }

    system("ls -vd ANS_DIR/*/*.stdout > ans_file.txt");
    read(ans_fd, ans_filepathbuf, BUFFER_SIZE);

    ptr = strtok(ans_filepathbuf, "\n");
    ans_idx = 0;
    while(ptr != NULL) {
        ans_filepathname[ans_idx] = ptr;
        ptr = strtok(NULL,"\n");
        //printf("%s\n",ans_filepathname[ans_idx]);
        ans_idx++;
    }
    close(ans_fd);
    system("rm ans_file.txt");

    for(ans_idx = 0 ; ans_idx < 4 ; ans_idx++) { //only extract file named " *.stdout " 
         ptr = strtok(ans_filepathname[ans_idx], "/");
         ans_count = 0;
        while(ptr != NULL) {
            if(ans_count == 2){
            stdout_filename[ans_idx] = ptr;
            //printf("%s\n",stdout_filename[ans_idx]); 
            }
            ans_count++;
            ptr = strtok(NULL,"/");
            
        }
    }

    // STD_DIR part 
    

    for(std_idx = 0 ; std_idx < 4 ; std_idx++) { 
        if((std_fd = open("std_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
            fprintf(stderr, "creat error for %s\n", "std_file.txt");
            exit(1);
        }    
        sprintf(ls_syntax, "%s %s %s%s %s %s", "ls", "-vd", "STD_DIR/*/" ,stdout_filename[std_idx],">","std_file.txt");
        //printf("%s\n", ls_syntax);
        system(ls_syntax);

        stdout_count = 0;
        read(std_fd, std_filepathbuf, BUFFER_SIZE);
        ptr = strtok(std_filepathbuf, "\n");
    
            while(ptr != NULL) {
                strcpy(std_filepathname[std_idx][stdout_count], ptr);
                //printf("%s\n",std_filepathname[std_idx][stdout_count]);
                stdout_count++;
                ptr = strtok(NULL,"\n");
            
            }
        lseek(std_fd, 0 ,SEEK_SET);
        close(std_fd);
    }
    
    system("rm std_file.txt");

    /*for(int i = 0 ; i<4; i++) {
        for(int j=0; j<20; j++) {
            printf("Result : %s\n", std_filepathname[i][j]);
        }
    }*/

    //Checking the Answer
    cfile_index = 45;
    for(ans_idx = 0 ; ans_idx < 4 ; ans_idx ++) {
        ans_fd = open(ans_filepathname[ans_idx], O_RDONLY);
        
        for(std_idx = 0 ;std_idx < 20 ; std_idx ++) {
            std_fd = open(std_filepathname[ans_idx][std_idx], O_RDONLY);
            ans_length = read(ans_fd, buf_ANS, BUFFER_SIZE);
            std_length = read(std_fd, buf_STD, BUFFER_SIZE);
                //printf("ANS : %s\n", buf_ANS);
                //printf("STD : %s\n", buf_STD);
            
                if(strcmp(buf_ANS, buf_STD) == 0)
                    continue;

                else    
                    total_score_tab_for[std_idx].score[cfile_index] = ERROR;

                //printf("%s :", std_filepathname[ans_idx][std_idx]);
                //printf("%.2lf\n",total_score_tab_for[std_idx].score[cfile_index]);

            for(int i=0 ; i< ans_length;i++)
                buf_ANS[i] = '\0';   
            for(int i=0 ; i< ans_length;i++)
                buf_STD[i] = '\0';      
            
            close(ans_fd);
            close(std_fd);
        }
        
        cfile_index ++;
    }
}
       

void blank_problem_check(char* directory_path_std, char* directory_path_ans) {
        
    struct  dirent **namelist;
    int ans_count, std_count;
    int cfile_index = 0;
    int txtfile_index = 0;
    int txt_count; // count of txt file
    int ans_idx, std_idx;
    int ans_length, std_length;
    int ans_fd, std_fd;
    char ls_syntax[50] = "\0"; // ls syntax
    char ans_filepathbuf[BUFFER_SIZE] = "\0"; //ANS_DIR buf of filepath
    char std_filepathbuf[BUFFER_SIZE] = "\0"; //STD_DIR buf of filepath
    char *txt_filename[45]; // only *.txt
    char *ans_filepathname[45]; // ANS_DIR/*/*.txt
    char std_filepathname[45][20][30]; // STD_DIR/*/*.txt
    char *ptr; //location pointer

    if((ans_fd = open("ans_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n", "ans_file.txt");
        exit(1);
    }

    system("ls -vd ANS_DIR/*/*.txt > ans_file.txt");
    read(ans_fd, ans_filepathbuf, BUFFER_SIZE);

    ptr = strtok(ans_filepathbuf, "\n");
    ans_idx = 0;
    while(ptr != NULL) {
        ans_filepathname[ans_idx] = ptr;
        ptr = strtok(NULL,"\n");
        //printf("%s\n",ans_filepathname[ans_idx]);
        ans_idx++;
    }
    close(ans_fd);
    system("rm ans_file.txt");

    for(ans_idx = 0 ; ans_idx < 45 ; ans_idx++) { //only extract file named " *.stdout " 
         ptr = strtok(ans_filepathname[ans_idx], "/");
         ans_count = 0;
        while(ptr != NULL) {
            if(ans_count == 2){
            txt_filename[ans_idx] = ptr;
            //printf("%s\n",stdout_filename[ans_idx]); 
            }
            ans_count++;
            ptr = strtok(NULL,"/");
            
        }
    }

    // STD_DIR part 
    

    for(std_idx = 0 ; std_idx < 45 ; std_idx++) { 
        if((std_fd = open("std_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
            fprintf(stderr, "creat error for %s\n", "std_file.txt");
            exit(1);
        }    
        sprintf(ls_syntax, "%s %s %s%s %s %s", "ls", "-vd", "STD_DIR/*/" ,txt_filename[std_idx],">","std_file.txt");
        //printf("%s\n", ls_syntax);
        system(ls_syntax);

        txt_count = 0;
        read(std_fd, std_filepathbuf, BUFFER_SIZE);
        ptr = strtok(std_filepathbuf, "\n");
    
            while(ptr != NULL) {
                strcpy(std_filepathname[std_idx][txt_count], ptr);
                //printf("%s\n",std_filepathname[std_idx][stdout_count]);
                txt_count++;
                ptr = strtok(NULL,"\n");
            
            }
        lseek(std_fd, 0 ,SEEK_SET);
        close(std_fd);
    }
    
    system("rm std_file.txt");

    /*for(int i = 0 ; i<45; i++) {
        for(int j=0; j<20; j++) {
            printf("Result : %s\n", std_filepathname[i][j]);
        }
    }*/

    //Checking the Answer
   
    for(ans_idx = 0 ; ans_idx < 45 ; ans_idx ++) {
        ans_fd = open(ans_filepathname[ans_idx], O_RDONLY);
        
        for(std_idx = 0 ;std_idx < 20 ; std_idx ++) {
            std_fd = open(std_filepathname[ans_idx][std_idx], O_RDONLY);
            ans_length = read(ans_fd, buf_ANS, BUFFER_SIZE);
            std_length = read(std_fd, buf_STD, BUFFER_SIZE);
                //printf("ANS : %s\n", buf_ANS);
                //printf("STD : %s\n", buf_STD);
            
                if(strcmp(buf_ANS, buf_STD) == 0)
                    continue;

                else    
                    total_score_tab_for[std_idx].score[ans_idx] = ERROR;

                //printf("%s :", std_filepathname[ans_idx][std_idx]);
                //printf("%.2lf\n",total_score_tab_for[std_idx].score[cfile_index]);

            for(int i=0 ; i< ans_length;i++)
                buf_ANS[i] = '\0';   
            for(int i=0 ; i< ans_length;i++)
                buf_STD[i] = '\0';      
            
            close(ans_fd);
            close(std_fd);
        }
        
    }
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

void score_table_create_proto(char *directory_path_std, char *directory_path_ans) {

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

    
    printf("%s\n", directory_path_std);
    if((std_count = scandir(directory_path_std, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_std, strerror(errno));
        exit(1);
        } 

    if((fd_total = open("score.csv", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
        fprintf(stderr, "creat error for %s\n","score.csv");
        exit(1);
    }
        
     for(idx = 0; idx < std_count; idx++) {
        if(strcmp(namelist[idx] -> d_name, ".") == 0 || strcmp(namelist[idx] -> d_name, "..") == 0) {
            except_dot++ ;
            continue;
        }
        strcpy(total_score_tab_for[idx].student, namelist[idx] -> d_name);
        printf("%s,",total_score_tab_for[idx].student);
        
    }

    for(idx = except_dot; idx < std_count; idx++) {
            strcpy(total_score_tab_for [idx - except_dot].student, total_score_tab_for[idx].student);
    }

    fd_backup = dup(1);
    dup2(fd_total , 1);

    printf(",");
    for(idx = 0; idx < pro_count; idx++) {
        printf("%s,", ssu_score_tab_for[idx].name);
    }
    printf("Sum\n");
    for(idx = 0; idx < std_count-except_dot; idx++) {
        
        total_sum = 0;
        printf("%s,",total_score_tab_for[idx].student);

        for(jdx = 0; jdx < pro_count ; jdx++){
        total_score_tab_for[idx].score[jdx] = ssu_score_tab_for[jdx].score;
        printf("%.2lf", total_score_tab_for[idx].score[jdx]);
        printf(",");
        total_sum += total_score_tab_for[idx].score[jdx];
        }
        print_sum_count[idx] = total_sum;
        printf("%.2lf\n", total_sum);
        
    }

   
    
    dup2(fd_backup, 1);
    close(fd_total);

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

    
    //printf("Checkpoint\n");
    if((std_count = scandir(directory_path_std, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_std, strerror(errno));
        exit(1);
        } 

    if((fd_total = open("score.csv", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
        fprintf(stderr, "creat error for %s\n","score.csv");
        exit(1);
    }

     for(idx = 0; idx < std_count; idx++) {
        if(strcmp(namelist[idx] -> d_name, ".") == 0 || strcmp(namelist[idx] -> d_name, "..") == 0) {
            except_dot++ ;
            continue;
        }
        
    }
        

    fd_backup = dup(1);
    dup2(fd_total , 1);

    printf(",");
    for(idx = 0; idx < pro_count; idx++) {
        printf("%s,", ssu_score_tab_for[idx].name);
    }
    printf("Sum\n");

    for(idx = 0; idx < std_count-except_dot; idx++) {
        
        total_sum = 0;
        printf("%s,",total_score_tab_for[idx].student);

        for(jdx = 0; jdx < pro_count ; jdx++){
        printf("%.2lf", total_score_tab_for[idx].score[jdx]);
        printf(",");
        total_sum += total_score_tab_for[idx].score[jdx];
        }
        print_sum_count[idx] = total_sum;
        printf("%.2lf\n", total_sum);
        
    }
    
    dup2(fd_backup, 1);
    close(fd_total);
    
    //printf("%s | %.2lf\n", total_score_tab_for[0].student, total_score_tab_for[0].score[0]);

    if(print_flag == 1) {
        for(idx = 0 ; idx < std_count - except_dot ;idx++) {
    
        printf("%s score : %.2lf\n", total_score_tab_for[idx].student, print_sum_count[idx]);
        print_sum_count_total += print_sum_count[idx];

        }

        avg = print_sum_count_total / (std_count - except_dot);
        printf("Total Average : %.2lf\n", avg);
    }

}

