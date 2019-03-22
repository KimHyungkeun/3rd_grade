#include "ssu_score.h"
#include "ssu_time.h"

void program_problem_check(char*, char*);
   DIR *dir__ans, *sub_ans, *dir_std, *sub_std; //dir_info : 처음 부모디렉터리, dir_* : DIR_* 내부의 디렉터리
   struct dirent  *entry_ans, *entry_sub_ans, *entry_std, *entry_sub_std; // dir_info_*를 readdir 하기 위한 변수, dir_*를 readdir 하기 위한 변수
   const char *sub_directory_ans; // ANS_DIR의 sub_directory 이름을 담는 변수
   const char *sub_directory_std; // STD_DIR의 sub_directory 이름을 담는 변수
   char dir_root_ans[30] = "ANS_DIR/"; //ANS_DIR  
   char dir_root_std[30] = "STD_DIR/"; //STD_DIR
   char dir_path_ans[30] = "\0"; //ANS_DIR  
   char dir_path_std[30] = "\0"; //STD_DIR
   char dir_path_backup_ans[30] = "\0"; // ANS_DIR/SUBDIR/로 돌아감
   char dir_path_backup_std[30] = "\0"; // STD_DIR/SUBDIR/로 돌아감
   char sub_path_ans_backup[30] = "\0";
   char sub_path_std_backup[30] = "\0";
   char buf_ANS[BUFFER_SIZE]; // ANS 의 파일내용을 담기 위한 변수
   char buf_STD[BUFFER_SIZE]; // STD의 파일내용을 담기 위한 변수
   int fd_ans, fd_std, count; //fd : 파일 디스크립터, count : 읽은 buf의 갯수
   time_t first, second;

int main(int argc, char *argv[])
{
   	
   char directory_path_ans[20] = "\0"; //ANS_DIR의 경로	   
   char directory_path_std[20] = "\0"; //STD_DIR의 경로
   struct timeval begin_t, end_t; //시작시간과 끝나는 시간	
   gettimeofday(&begin_t, NULL);

   strcpy(directory_path_std, argv[1]);
   strcpy(directory_path_ans, argv[2]); 
   strcat(directory_path_std, "/");
   strcat(directory_path_ans, "/");

   if(argc != 3) {
	fprintf(stderr,"Usage : %s STD_DIR ANS_DIR\n",argv[0]);
	gettimeofday(&end_t,NULL);
       	ssu_runtime(&begin_t, &end_t);
	exit(1);
	}
	
	//blank_problem_check(directory_path_std, directory_path_ans); //빈칸 채우기 문제 	
	//program_problem_check(directory_path_std, directory_path_ans); //프로그래밍 문제
	score_table_create(directory_path_ans);

	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(0);

}

void program_problem_check(char* directory_path_std, char* directory_path_ans){
    struct  dirent **namelist;
    int     count;
    int     idx;
    int     stdoutfile_fd, fd_backup, fd_backup2;
    char exe_syntax[30] = "\0";
    char gcc_syntax[30] = "\0";
    char run_syntax[30] = "\0";
    char stdoutfile_name[30] = "\0";
    char buf[BUFFER_SIZE];
    
    if((count = scandir(directory_path_ans, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_ans, strerror(errno));
        exit(1);
    }


    for(idx = 0; idx < count; idx++) {
        if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
        continue;

        else {
        strcpy(dir_path_backup_ans, directory_path_ans);
        strcat(directory_path_ans, namelist[idx] -> d_name);
        strcat(directory_path_ans, "/");
        strcpy(sub_path_ans_backup, directory_path_ans); // ANS_DIR/SUBDIR

        sub_ans = opendir(directory_path_ans);
        while( entry_sub_ans = readdir(sub_ans)){

            if(strcmp(entry_sub_ans -> d_name,".") == 0 || strcmp(entry_sub_ans-> d_name,"..") == 0)
            continue;

            if(strpbrk(entry_sub_ans -> d_name,"-") != NULL || strpbrk(entry_sub_ans -> d_name,".csv"))
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
                sprintf(gcc_syntax, "%s %s%s %s %s%s", "gcc" , directory_path_ans,".c", "-o", exe_syntax,".exe");
                system(gcc_syntax);

                
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
        /*printf("%s\n", namelist[idx]-> d_name);
        strcat(directory_path_ans, namelist[idx] -> d_name);
        fd = open(path, O_RDONLY);
        fd_count = read(fd, buf, BUFFER_SIZE); //처음부터 시작하여 1024byte 만큼을 읽는다.
        printf("%d bytes\n", fd_count);
		buf[fd_count] = 0;
        strcpy(path, path_backup);
        close(fd);*/

        }
    }
   
    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    free(namelist);

    // STD_DIR auto compile
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
        strcpy(sub_path_std_backup, directory_path_std); // STD_DIR/STDNUM

        sub_std = opendir(directory_path_std);
        while( entry_sub_std = readdir(sub_std)){

            if(strcmp(entry_sub_std -> d_name,".") == 0 || strcmp(entry_sub_std-> d_name,"..") == 0)
            continue;

            if(strpbrk(entry_sub_std -> d_name,"-") != NULL )
            continue;

            else {
            strcat(directory_path_std, entry_sub_std -> d_name);
    
            sprintf(exe_syntax, "%s%s" , sub_path_std_backup, entry_sub_std -> d_name);
            printf("exe_syntax %s\n", exe_syntax);

            sprintf(stdoutfile_name, "%s%s", exe_syntax, ".stdout");
            printf("stdoutfile_name %s\n", stdoutfile_name);
            
            
            if( (stdoutfile_fd = open(stdoutfile_name, O_RDWR | O_CREAT | O_TRUNC, 0640)) < 0) {
                    fprintf(stderr, "open error for %s\n", stdoutfile_name);
                    exit(1);
            }
                fd_backup = dup(1);
                fd_backup2 = dup(2);
                sprintf(gcc_syntax, "%s %s %s %s%s", "gcc" , directory_path_std, "-o", exe_syntax,".exe");
                system(gcc_syntax);

                dup2(stdoutfile_fd, 2);
                dup2(stdoutfile_fd, 1);

                
                first = time(NULL);
                sprintf(run_syntax, "%s%s%s", "./",exe_syntax, ".exe");
                system(run_syntax);
                second = time(NULL);

                dup2(fd_backup, 1);
                dup2(fd_backup2, 2);
                printf("wait time is %f seconds\n\n", difftime(second, first));
                close(stdoutfile_fd); 

             strcpy(directory_path_std, sub_path_std_backup );
            }
            
        }
        closedir(sub_std); 
        strcpy(directory_path_std,dir_path_backup_std);
        /*printf("%s\n", namelist[idx]-> d_name);
        strcat(directory_path_ans, namelist[idx] -> d_name);
        fd = open(path, O_RDONLY);
        fd_count = read(fd, buf, BUFFER_SIZE); //처음부터 시작하여 1024byte 만큼을 읽는다.
        printf("%d bytes\n", fd_count);
		buf[fd_count] = 0;
        strcpy(path, path_backup);
        close(fd);*/

        }
    }
   
    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    
    free(namelist);
    
}

void score_table_create(char* directory_path_ans) {

	
    struct  dirent **namelist;
 	int fd_score;//fd : 파일 디스크립터, count : 읽은 buf의 갯수
    int idx;
	
	


   if((count = scandir(directory_path_ans, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_ans, strerror(errno));
        exit(1);
        } 

    struct table_format tab_for[49]; //table format

     if((fd_score = open("ANS_DIR/score_table.csv",O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
	    fprintf(stderr,"open error for %s\n","ANS_DIR/score_table.csv");
	    exit(1);
        }	

        for(idx = 0; idx < count; idx++) {
            if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
            continue;

        else {
        printf("Input of ");
	    printf("%s : ",namelist[idx] -> d_name);
        scanf("%lf",&tab_for[idx].score);
        tab_for[idx].comma = ',';
        tab_for[idx].linejump = '\n';
	    strcpy(tab_for[idx].name, namelist[idx] -> d_name);
	    write(fd_score, (char *)&tab_for, sizeof(tab_for));
        }
    }


  close(fd_score);
  exit(0);

}