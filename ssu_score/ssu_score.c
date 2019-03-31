#include "ssu_score.h"
#include "ssu_time.h"

int main(int argc, char *argv[])
{
   int opt;
   int qname_flag = 0; //t 옵션
   int error_flag = 0; //e 옵션
   int print_flag = 0; //p 옵션
   int help_flag = 0; // h 옵션
   char t_filename[20]; //t 옵션에 들어갈 파일이름
   char e_filename[20];	//e 옵션에 들어갈 파일이름
   char directory_path_ans[20] = "\0"; //ANS_DIR의 경로	   
   char directory_path_std[20] = "\0"; //STD_DIR의 경로
   gettimeofday(&begin_t, NULL);

   if(argc < 3) { //인자가 3개 미만이면 종료
	fprintf(stderr,"Usage : %s STD_DIR ANS_DIR [option]\n",argv[0]);
	gettimeofday(&end_t,NULL);
    	ssu_runtime(&begin_t, &end_t);
	exit(1);
	}

   strcpy(directory_path_std, argv[1]);
   strcat(directory_path_std, "/"); // "ANS_DIR/" 형태로 만든다
   strcpy(directory_path_ans, argv[2]); 
   strcat(directory_path_ans, "/"); // "STD_DIR/" 형태로 만든다

   while ((opt = getopt(argc, argv, "phe:t: ")) != -1) { //옵션은 p,h,e,t 4종류가 있다
    switch(opt) {

        case 'h' : //help 옵션
            help_flag = 1;
            break;
         
        case 't' : //lpthread 컴파일 옵션
            qname_flag = 1;
            for(int i = 0 ; i < argc ; i++) {
                if(strcmp(argv[i], "-t") == 0) {
                    strcpy(t_filename, argv[i+1]);
                    memcpy(t_filename, optarg, strlen(optarg)); //t 옵션 다음의 file에 대해 다룬다
                    break;
                }
            }
            break;
        
        case 'e' : //error 폴더 생성 옵션
            error_flag = 1;
            for(int i = 0 ; i < argc ; i++) {
                if(strcmp(argv[i], "-e") == 0) {
                    strcpy(e_filename, argv[i+1]); 
                    memcpy(e_filename, optarg, strlen(optarg)); // error파일을 담을 폴더명을 지정한다 
                    break;
                }
            }
            break;

        case 'p' : //각 학생들의 score와 전체 평균을 출력하는 옵션이다.
            print_flag = 1;
            break;

        case '?': //파일 인자를 필요로 하는 옵션에서 유효하지 않은 값을 입력하면 프로그램을 종료시킨다.
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

   if(help_flag) { //help 옵션을 작동시키면 사용법을 보여주고 프로그램을 종료시킨다.
	fprintf(stderr,"Usage : %s STD_DIR ANS_DIR [option]\n-e <DIRNAME>   print error on 'DIRNAME/ID/qname_error.txt' file\n-t <QNAME>   compile QNAME.c with -lpthread option\n-h  print usage\n-p  print student's score and total average\n",argv[0]);
	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t);
	exit(1);
	}

    

    ssu_score_table_create(directory_path_ans); //ssu_score.csv 파일을 만드는 함수
    score_table_create_proto(directory_path_std, directory_path_ans); //score.csv를 만드는 함수
    program_autocompile(directory_path_std, directory_path_ans, qname_flag, error_flag, t_filename, e_filename); //프로그래밍 문제 자동 컴파일
    program_problem_check(directory_path_std, directory_path_ans); //프로그래밍 문제 채점
    blank_problem_check(directory_path_std, directory_path_ans); //빈칸 채우기 문제 채점 	
    score_table_create(directory_path_std, directory_path_ans, print_flag); //채점 후의 최종 score.csv를 생성
    

	gettimeofday(&end_t,NULL);
    ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}

void program_autocompile(char* directory_path_std, char* directory_path_ans, int qname_flag, int error_flag, char* t_filename, char* e_filename){
    struct  dirent **namelist, **stdnamelist;
    int     count, except = 0; //count : scandir로 측정한 폴더갯수
    int     std_count=0; //stddir 폴더 갯수
    int     err_dir_count = 0;
    int     idx; //index로 쓸 변수
    int     stdoutfile_fd, error_fd, fd_backup, fd_backup2; //stdoutfile과 error file을 만들기 위한 파일 디스크립터
    int not_c; //c파일이 아님을 구별하기 위한 변수
    int ptr_count; //포인터 변수 에서 토큰 기준으로 몇번 점프 했는지 카운팅
    char root_add_filename[10] = "\0";  // "/" + "filename" = "/filename" 형태로 만듦
    char token_divide[30] = "\0"; 
    char exe_filename[30] = "\0"; //실행 파일 이름
    char stdout_filename[30] = "\0"; //stdout 파일 이름
    char txt_filename[30] = "\0"; //txt 파일 이름
    char exe_syntax[30] = "\0"; //.exe 구문으로 만듦
    char gcc_syntax[30] = "\0"; //gcc ... 구문으로 만듦
    char run_syntax[30] = "\0"; //exe를 실행하기 위한 구문을 만듦
    char stdoutfile_name[30] = "\0"; //stdout 파일 이름
    char errorfile_name[30] = "\0"; //에러파일 이름
    char killall_command[30] = "\0"; //동작중인 프로세스를 kill하기 위한 구문
    char remove_error_directory[30] = "\0"; //error 디렉터리 제거
    char make_error_directory[30] = "\0"; //error 디렉터리 생성
    char move_error_file_syntax[50] = "\0";//error파일을 error 디렉터리로 움직이기위한 변수
    char *ptr; // 토큰 분할 시 사용하는 임시 포인터 변수
    int compile_count = 0;

   
    
    // ANS_DIR 자동 컴파일 부분
    if((count = scandir(directory_path_ans, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_ans, strerror(errno));
        exit(1);
    }
    
    for(idx = 0; idx < count; idx++) {
        if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
            continue; // .이나 ..이 발견되면 생략한다
        
        if(strcmp(namelist[idx] -> d_name, "score_table.csv") == 0 || strpbrk(namelist[idx] -> d_name,"-") != NULL)
            continue; // score_table.csv이나 txt 문제들은 생략한다

        else {
        strcpy(dir_path_backup_ans, directory_path_ans);
        strcat(directory_path_ans, namelist[idx] -> d_name); //"ANS_DIR/" 형태로 돌아옴
        strcat(directory_path_ans, "/");
        strcpy(sub_path_ans_backup, directory_path_ans); // "ANS_DIR/SUBDIR"형태로 돌아옴

        sub_ans = opendir(directory_path_ans);
        while( entry_sub_ans = readdir(sub_ans)){

            if(strcmp(entry_sub_ans -> d_name,".") == 0 || strcmp(entry_sub_ans-> d_name,"..") == 0)//.이나 ..은 생략
            continue;

            sprintf(exe_filename,"%s%s",namelist[idx] -> d_name,".exe");
            if(strcmp(exe_filename,entry_sub_ans -> d_name) == 0) // ANS_DIR/SUB_DIR/*.exe 형태
                continue;
            sprintf(stdout_filename,"%s%s",namelist[idx] -> d_name,".stdout");
            if(strcmp(stdout_filename,entry_sub_ans -> d_name) == 0) // ANS_DIR/SUB_DIR/*.stdout 형태
                continue;
            
            else {
            strcat(directory_path_ans, namelist[idx] -> d_name);
    
            sprintf(exe_syntax, "%s%s" , sub_path_ans_backup, namelist[idx] -> d_name);
            printf("exe_syntax %s\n", exe_syntax); // ANS_DIR/SUB_DIR/*.exe 형태

            sprintf(stdoutfile_name, "%s%s", exe_syntax, ".stdout");
            printf("stdoutfile_name %s\n", stdoutfile_name); // ANS_DIR/SUB_DIR/*.stdout 형태
            
            
            if( (stdoutfile_fd = open(stdoutfile_name, O_RDWR | O_CREAT | O_TRUNC, 0640)) < 0) {
                    fprintf(stderr, "open error for %s\n", stdoutfile_name);
                    exit(1);
            }
                fd_backup = dup(1); //표준출력 디스크립터 백업
              
                sprintf(gcc_syntax, "%s %s%s %s %s %s%s", "gcc" , directory_path_ans,".c", "-lpthread","-o", exe_syntax,".exe");
                system(gcc_syntax); //gcc 로 컴파일 실시

                dup2(stdoutfile_fd, 1); //stdout 파일에 컴파일 내용 담음

                
                first = time(NULL);
                sprintf(run_syntax, "%s%s%s", "./",exe_syntax, ".exe");
                system(run_syntax); //프로그램 실행
                second = time(NULL);

                dup2(fd_backup, 1); //백업한 디스크립터 복원
                printf("wait time is %f seconds\n\n", difftime(second, first));
                close(stdoutfile_fd); 

             strcpy(directory_path_ans, sub_path_ans_backup ); //다시 ANS_DIR/ 형태로 되돌아간다
            }
            
        }
        closedir(sub_ans); 
        strcpy(directory_path_ans,dir_path_backup_ans);
       
        }

    }
   
    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]); //scandir 사용후 할당했던 메모리를 다시 반환한다
    }

    free(namelist);

    // STD_DIR 자동 컴파일 부문 
    
    if((count = scandir(directory_path_std, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", directory_path_std, strerror(errno));
        exit(1);
    }

    for(idx = 0; idx < count; idx++) {
        if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
            continue; //. 이나 ..을 만나면 생략하고 다음 디렉터리로 넘어간다

        else {
        strcpy(dir_path_backup_std, directory_path_std);
        strcat(directory_path_std, namelist[idx] -> d_name); // STD_DIR/ 형태
        strcat(directory_path_std, "/");
        strcpy(sub_path_std_backup, directory_path_std); // STD_DIR/SUBDIR 형태

        sub_std = opendir(directory_path_std);
        while( entry_sub_std = readdir(sub_std)){
            
            if(strcmp(entry_sub_std -> d_name,".") == 0 || strcmp(entry_sub_std-> d_name,"..") == 0)
            continue; //.이나 ..을 만나면 생략하고 다음 파일을 읽음

        
            if(strpbrk(entry_sub_std -> d_name, "-") != NULL)
                continue; //txt파일을 만나면 생략하고 다음 파일을 읽음

            for(int i=0 ; i < strlen(entry_sub_std -> d_name) ; i++) {
                
                if(entry_sub_std -> d_name [i] == 'e' || entry_sub_std -> d_name [i]== 's') {
                    not_c = 1; //exe나 stdout을 생략하기 위한 조건문
                }

            }
            
            if(not_c == 1) {
                not_c = 0; //만약 exe나 stdout파일이면 다음 파일로 넘어간다
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
                    directory_path_std[i] = '\0'; //.stdout이나 .exe 라는 이름을 붙이기위해 .c라는 이름을 뗀다
            }

          
            printf("directory_path_std : %s\n", directory_path_std);

            sprintf(exe_syntax, "%s" , directory_path_std);
            printf("exe_syntax %s\n", exe_syntax);

            sprintf(stdoutfile_name, "%s%s", exe_syntax, ".stdout"); // *.exe 형태
            printf("stdoutfile_name %s\n", stdoutfile_name);

            sprintf(errorfile_name, "%s%s", exe_syntax, "_error.txt"); // *.stdout 형태
            printf("errorfile_name %s\n\n", errorfile_name);

            
            ptr = strstr(directory_path_std,"/12"); // "/12"라는 패턴을 문자열에서 찾는다.
            if(ptr == NULL)
                ptr = "null"; //못찾으면 null이라는 문자열 대입
            sprintf(root_add_filename, "%s%s", "/", t_filename);
            
            
            if( (stdoutfile_fd = open(stdoutfile_name, O_RDWR | O_CREAT | O_TRUNC, 0755)) < 0 ) {
                    fprintf(stderr, "open error for %s\n", stdoutfile_name); //stdout 파일 생성
                    exit(1);
            }

            if( (error_fd = open(errorfile_name, O_RDWR | O_CREAT | O_TRUNC, 0755)) < 0 ) {
                    fprintf(stderr, "open error for %s\n", errorfile_name); //error 파일 생성
                    exit(1);
                 }

                    fd_backup = dup(1); //표준출력 디스크립터 백업
                    fd_backup2 = dup(2); //표준에러 디스크립터 백업
                    dup2(stdoutfile_fd, 1); 
                    dup2(error_fd,2);
                    

                    if(qname_flag == 1 && strcmp(root_add_filename, ptr) == 0) { //t 옵션에 의해 lpthread 옵션이 들어간 컴파일 수행
                        sprintf(gcc_syntax, "%s %s%s %s %s %s%s", "gcc" , directory_path_std,".c", "-lpthread","-o", exe_syntax,".exe");
                    }
                    else { //만약 아니면 lpthread 옵션 없이 수행
                        sprintf(gcc_syntax, "%s %s%s %s %s%s", "gcc" , directory_path_std,".c", "-o", exe_syntax,".exe");
                    }

                    system(gcc_syntax); //gcc 수행

                
                    sprintf(killall_command,"%s %s%s", "killall", exe_syntax,"*"); //kill 명령어 수행을 휘한 구문
                    sprintf(run_syntax, "%s%s%s %s", "./",exe_syntax, ".exe","&"); //exe를 실행을 위한 구문


                    system(run_syntax); //프로그램 실행
                
                
                    dup2(fd_backup, 1); // 표준출력 디스크립터 복원
                    dup2(fd_backup2,2); // 표준에러 디스크립터 복원
                    close(stdoutfile_fd); 
                    close(error_fd);
            


             strcpy(directory_path_std, sub_path_std_backup ); // STD_DIR/SUBDIR/ 형태로 되돌아감
            }
            
        }
        closedir(sub_std); 
        strcpy(directory_path_std,dir_path_backup_std);
       
        }
    }
        

    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]); //scandir 사용 후 할당한 메모리를 반환한다
    }

    free(namelist);

  
    system("ps -ef | grep STD_DIR/"); //끝나지않고 돌고있는 프로세스들을 보여줌
    printf("Please wait... 10 secs \n"); //10초 대기
    sleep(10);
    printf("kill process when proceed time limits 5 sec or stopped\n"); 
    system("killall 12.exe"); //12.exe 파일 죽이기
    system("killall 13.exe"); //13.exe 파일 죽이기

    
     //error 옵션 작동 과정
     if(error_flag == 1) {
        sprintf(remove_error_directory,"%s %s","rm -rf",e_filename); // 모든 서브 디렉터리를 지우기 위한 구문
        sprintf(make_error_directory,"%s %s","mkdir",e_filename); // 디렉터리를 새로 생성하기 위한 구문

        if((count = scandir(".", &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", "ssu_score", strerror(errno));
        exit(1);
        }

        for(idx = 0; idx < count; idx++) {
            if(strcmp(namelist[idx] -> d_name, e_filename) == 0 )
                system(remove_error_directory); // 만약 존재하는 디렉터리면 지우고 다시 만들기 위함
        }

        if((std_count = scandir("STD_DIR/", &stdnamelist, NULL, alphasort)) == -1) { //STD_DIR 내부 디렉터리 확인
        fprintf(stderr, "%s Directory Scan Error: %s\n","STD_DIR", strerror(errno));
        exit(1);
        }

        mkdir(e_filename, 0775); //사용자가 지정한 이름으로 에러 디렉터리 생성

        for(int i = 2; i < std_count; i++) { // move all error file to "error" directory from 20190001 ~ 20190020
                sprintf(make_error_directory , "%s%s%s", e_filename,"/",stdnamelist[i] -> d_name);
                mkdir(make_error_directory, 0775);
        }

        for(int i = 2; i < std_count; i++) {
                sprintf(directory_path_std, "%s%s%s%s%s", "STD_DIR","/",stdnamelist[i] -> d_name,"/", "*_error.txt");
                sprintf(make_error_directory , "%s%s%s%s", e_filename,"/",stdnamelist[i] -> d_name,"/");
                sprintf(move_error_file_syntax, "%s %s %s", "mv", directory_path_std, make_error_directory); //mv STD_DIR/SUBDIR/*.error DIRNAME/SUBDIR/
                system(move_error_file_syntax); //각 STD_DIR에 있는 error파일들을 새로 생성한 error 파일들에 다 넣는다
        }

        strcpy(directory_path_std,dir_path_backup_std); // "STD_DIR/" 형태로 되돌려 놓는다

         for(idx = 0; idx < std_count; idx++) {
            free(stdnamelist[idx]); //scandir 사용 후 할당한 메세지를 다시 반환
        }
            free(stdnamelist);

         for(idx = 0; idx < count; idx++) {
            free(namelist[idx]); //scandir 사용 후 할당한 메세지를 다시 반환
        }
        free(namelist);
    
    }

    
    
}

void program_problem_check(char* directory_path_std, char* directory_path_ans) { 
    struct  dirent **namelist;
    int ans_count, std_count; 
    int cfile_index = 0; //c파일을 세기위한 인덱스
    int stdout_count;
    int ans_idx, std_idx; //ans 디렉터리 전용 인덱스, std 디렉터리 전용 인덱스
    int ans_length, std_length; //ans내의 파일의 총 길이, std내의 파일의 총 길이
    int ans_fd, std_fd; //ans_fd : ans 디렉터리에 대한 파일디스크립터, std_fd : std 디렉터리에 대한 파일 디스크립터 
    char ls_syntax[50] = "\0"; // ls syntax
    char txt_filename[30] = "\0"; // txt 파일이름
    char ans_filepathbuf[BUFFER_SIZE] = "\0"; //ANS_DIR buf of filepath
    char std_filepathbuf[BUFFER_SIZE] = "\0"; //STD_DIR buf of filepath
    char *stdout_filename[4]; // only *.stdout 
    char *ans_filepathname[4]; // ANS_DIR/*/*.stdout
    char std_filepathname[4][20][30]; // STD_DIR/*/*.stdout
    char *ptr; //location pointer

   

    if((ans_fd = open("ans_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n", "ans_file.txt");
        exit(1); //임시 txt파일 생성
    }

    system("ls -vd ANS_DIR/*/*.stdout > ans_file.txt");
    read(ans_fd, ans_filepathbuf, BUFFER_SIZE);

    ptr = strtok(ans_filepathbuf, "\n"); //개행문자에 따라 각 pathname들을 나눈다
    ans_idx = 0;
    while(ptr != NULL) {
        ans_filepathname[ans_idx] = ptr; //얻은 pathname을 저장
        ptr = strtok(NULL,"\n");
        ans_idx++;
    }
    close(ans_fd);
    system("rm ans_file.txt"); //임시 txt파일 삭제

    for(ans_idx = 0 ; ans_idx < 4 ; ans_idx++) { // " *.stdout " 이름을 가진 파일만 추출하도록 한다
         ptr = strtok(ans_filepathname[ans_idx], "/");
         ans_count = 0;
        while(ptr != NULL) {
            if(ans_count == 2){
            stdout_filename[ans_idx] = ptr; // *.stdout으로 된 파일들의 항목이 생성된다
            }
            ans_count++;
            ptr = strtok(NULL,"/");
            
        }
    }

    // STD_DIR part 
    

    for(std_idx = 0 ; std_idx < 4 ; std_idx++) { //c파일 문제가 4개이므로 
        if((std_fd = open("std_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
            fprintf(stderr, "creat error for %s\n", "std_file.txt");
            exit(1);
        }    
        sprintf(ls_syntax, "%s %s %s%s %s %s", "ls", "-vd", "STD_DIR/*/" ,stdout_filename[std_idx],">","std_file.txt");
        system(ls_syntax); //디렉터리 항목들을 순서대로 받아온다

        stdout_count = 0;
        read(std_fd, std_filepathbuf, BUFFER_SIZE);
        ptr = strtok(std_filepathbuf, "\n"); //개행문자에 따라 pathname을 구별한다
    
            while(ptr != NULL) {
                strcpy(std_filepathname[std_idx][stdout_count], ptr); //각 학번별의 stdout을 정답 stdout과 비교하기 위한 형태
                stdout_count++;
                ptr = strtok(NULL,"\n");
            
            }
        lseek(std_fd, 0 ,SEEK_SET);
        close(std_fd);
    }
    
    system("rm std_file.txt");


    //Checking the Answer
    cfile_index = 45; //scoretable내에서 프로그래밍 문제는 index 45 ~ 48 까지
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
                    total_score_tab_for[std_idx].score[cfile_index] = ERROR; //값이 다르므로 오답처리

                //printf("%s :", std_filepathname[ans_idx][std_idx]);
                //printf("%.2lf\n",total_score_tab_for[std_idx].score[cfile_index]);

            for(int i=0 ; i< ans_length;i++)
                buf_ANS[i] = '\0';   //다쓴 버퍼는 다시 null문자로 초기화
            for(int i=0 ; i< ans_length;i++)
                buf_STD[i] = '\0';   //다쓴 버퍼는 다시 null문자로 초기화   
            
            close(ans_fd);
            close(std_fd);
        }
        
        cfile_index ++; //scoretable내에서 프로그래밍 문제는 index 45 ~ 48 까지 
    }



}
       

void blank_problem_check(char* directory_path_std, char* directory_path_ans) {
        
    struct  dirent **namelist;
    int ans_count, std_count;
    int cfile_index = 0;
    int txtfile_index = 0;
    int txt_count; // count of txt file
    int ans_idx, std_idx;
    int ans_length, std_length; //buffer내에 담긴 내용의 길이, ans와 std 각각 디렉터리에 대해 다룬다
    int ans_fd, std_fd; //파일 디스크립터, ans와 std 각각 디렉터리에 대해 다룬다
    char ls_syntax[50] = "\0"; // ls syntax
    char ans_filepathbuf[BUFFER_SIZE] = "\0"; //ANS_DIR buf of filepath
    char std_filepathbuf[BUFFER_SIZE] = "\0"; //STD_DIR buf of filepath
    char *txt_filename[45]; // only *.txt
    char *ans_filepathname[45]; // ANS_DIR/SUBDIR/*.txt
    char std_filepathname[45][20][30]; // STD_DIR/SUBDIR/*.txt
    char *ptr; // strtok함수에 쓰일 임시 포인터 변수

    if((ans_fd = open("ans_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n", "ans_file.txt");
        exit(1);
    }

    system("ls -vd ANS_DIR/*/*.txt > ans_file.txt");
    read(ans_fd, ans_filepathbuf, BUFFER_SIZE); 

    ptr = strtok(ans_filepathbuf, "\n"); //개행문자에 따라 pathname을 나눈다
    ans_idx = 0;
    while(ptr != NULL) {
        ans_filepathname[ans_idx] = ptr; //pathname들을 배열에 저장해둔다. 
        ptr = strtok(NULL,"\n");
        ans_idx++;
    }
    close(ans_fd);
    system("rm ans_file.txt");

    for(ans_idx = 0 ; ans_idx < 45 ; ans_idx++) { //only extract file named " *.stdout " 
         ptr = strtok(ans_filepathname[ans_idx], "/");
         ans_count = 0;
        while(ptr != NULL) {
            if(ans_count == 2){
            txt_filename[ans_idx] = ptr; //.txt 파일만 있는 쪽만 골라낸다
            }
            ans_count++;
            ptr = strtok(NULL,"/");
            
        }
    }

    // STD_DIR part 
    

    for(std_idx = 0 ; std_idx < 45 ; std_idx++) { //txt 비교문서는 scoretable에서 0 ~ 44까지
        if((std_fd = open("std_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
            fprintf(stderr, "creat error for %s\n", "std_file.txt");
            exit(1); //임시 txt 파일 생성
        }    
        sprintf(ls_syntax, "%s %s %s%s %s %s", "ls", "-vd", "STD_DIR/*/" ,txt_filename[std_idx],">","std_file.txt");
        system(ls_syntax); //STD내의 txt파일 항목들을 순서대로 불러옴

        txt_count = 0;
        read(std_fd, std_filepathbuf, BUFFER_SIZE);
        ptr = strtok(std_filepathbuf, "\n"); //개행문자에 따라 pathname을 나눈다
    
            while(ptr != NULL) {
                strcpy(std_filepathname[std_idx][txt_count], ptr); //각 학번별로 가지고있는 txt파일들을 배열에 저장
                txt_count++;
                ptr = strtok(NULL,"\n");
            
            }
        lseek(std_fd, 0 ,SEEK_SET);
        close(std_fd);
    }
    
    system("rm std_file.txt"); //임시 txt 파일 제거


    //Checking the Answer
   
    for(ans_idx = 0 ; ans_idx < 45 ; ans_idx ++) {
        ans_fd = open(ans_filepathname[ans_idx], O_RDONLY);
        
        for(std_idx = 0 ;std_idx < 20 ; std_idx ++) {
            std_fd = open(std_filepathname[ans_idx][std_idx], O_RDONLY);
            ans_length = read(ans_fd, buf_ANS, BUFFER_SIZE); //정답지의 내용
            std_length = read(std_fd, buf_STD, BUFFER_SIZE); //학생이 제출한 내용
                //printf("ANS : %s\n", buf_ANS);
                //printf("STD : %s\n", buf_STD);
            
                if(strcmp(buf_ANS, buf_STD) == 0)
                    continue;

                else {   
                    total_score_tab_for[std_idx].score[ans_idx] = ERROR; //답이 아니므로 오답처리
                }

                //printf("%s :", std_filepathname[ans_idx][std_idx]);
                //printf("%.2lf\n",total_score_tab_for[std_idx].score[cfile_index]);

            for(int i=0 ; i< ans_length;i++)
                buf_ANS[i] = '\0';   //끝난 buf에 대해서는 null문자로 초기화
            for(int i=0 ; i< ans_length;i++)
                buf_STD[i] = '\0';   //끝난 buf에 대해서는 null문자로 초기화
            
            close(ans_fd);
            close(std_fd);
        }
        
    }
}

void ssu_score_table_create(char* directory_path_ans) { //ssu_score.csv 파일 만들기

	
    struct  dirent **namelist;
    int select_mode; //score table 생성시 모드를 선택
    int fd_score;//fd : 파일 디스크립터, count : 읽은 buf의 갯수
    int fd_tmp; //임시로 디스크립터를 담기 위한 변수
    int idx, nameptr_count; 
    int fd_backup, fd_backup2; //파일디스크립터 백업을 위한 변수
    int except_dot=0, except_csv=0, except_sum = 0; // ".", "csv" 를 제외하고 카운팅 하기  위한 변수
    double txt_score, c_score; //txt에 대한 점수, c파일에 대한 점수
    char *ptr, *name_ptr;
	
   
    if((fd_score = open("ANS_DIR/score_table.csv", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n","score_table.csv");
        exit(1); //csv 파일 생성
    }

    if((fd_tmp = open("dir.txt", O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
        fprintf(stderr, "creat error for %s\n","dir.txt");
        exit(1); //임시 txt 파일 생성
    }
    
    system("ls -vd ANS_DIR/*/ > dir.txt");
    read(fd_tmp, buf_dirname, BUFFER_SIZE); //ANS_DIR 내의 파일들을 정렬된 순서대로 읽어들인다
    close(fd_tmp);
    system("rm dir.txt"); //임시 txt 파일 삭제

    ptr = strtok(buf_dirname, "\n");
    while(ptr != NULL) {
        strcpy(ssu_score_tab_for[pro_count].name, ptr); //받은 항목들을순서대로 배열에 저장한다 
        pro_count++;
        ptr = strtok(NULL,"\n");
    }

    for(idx = 0 ; idx < pro_count; idx++) {
        name_ptr = strtok(ssu_score_tab_for[idx].name, "/"); // "/" 기준으로 pathname 구별
        nameptr_count = 0;
            while(name_ptr != NULL) {
            
            if(nameptr_count == 1) { 
                strcpy(ssu_score_tab_for[idx].name, name_ptr);
                break;
            }
            nameptr_count ++;
            name_ptr = strtok(NULL,"/");
        }
    }

        
    printf("Select Mode(1 or 2) : "); //사용자가 점수를 어떻게 입력할 것인지 
    scanf("%d", &select_mode);

    while(1) {
    if(select_mode == 1){ //1번 모드 지정

        printf("Input score for txt_file : ");
        scanf("%lf",&txt_score); // txt 문제 점수 지정
        printf("Input score for c_file : ");
        scanf("%lf",&c_score); // c 문제 점수  지정
        for(idx = 0; idx< pro_count ; idx++) {
           
            
            if(strpbrk(ssu_score_tab_for[idx].name, "-") != NULL)
            ssu_score_tab_for[idx].score = txt_score;  //모든 txt문제에 점수 지정

            else
            ssu_score_tab_for[idx].score = c_score; //모든 c문제에 점수 지정
        
            ssu_score_tab_for[idx].comma = ','; //구분자는 ","
            ssu_score_tab_for[idx].linejump = '\n'; //라인 구분자는 "\n"
        }
        break;
    }

    else if(select_mode == 2) { //2번 모드 발동
     for(idx = 0; idx< pro_count ; idx++) {
            printf("Input of ");
	        printf("%s",ssu_score_tab_for[idx].name);
        
            if(strpbrk(ssu_score_tab_for[idx].name, "-") != NULL)
            printf("%s :",".txt"); // txt문제는 *.txt 형식으로 이름지어짐
            else
            printf("%s :",".c"); // c문제는 *.c 형식으로 이름지어짐
            
            scanf("%lf",&ssu_score_tab_for[idx].score);
            ssu_score_tab_for[idx].comma = ',';
            ssu_score_tab_for[idx].linejump = '\n';
        }
        break;
    }

        else {
        printf("Select Mode(1 or 2) Retry : "); //다른 값 입력시 재 입력
        scanf("%d", &select_mode);
        }

    }

       fd_backup = dup(1);
       dup2(fd_score,1);
       for(idx = 0 ; idx < pro_count; idx++) {
           printf("%s%c%.2lf%c", ssu_score_tab_for[idx].name,ssu_score_tab_for[idx].comma,ssu_score_tab_for[idx].score,ssu_score_tab_for[idx].linejump); //입력한 모든값들을 ssu_score.csv에 출력한다.
       }
       dup2(fd_backup,1);
       close(fd_score);
}

void score_table_create_proto(char *directory_path_std, char *directory_path_ans) {

    struct dirent **namelist;
    int std_count, except_dot = 0;
    int idx, jdx;
    int fd_total, fd_backup, fd_backup2; //
    double  total_sum, avg = 0; //전체점수, 평균
    double  print_sum_count[20];
    double  print_sum_count_total = 0;
    char exe_syntax[30] = "\0"; //exe 구문
    char gcc_syntax[30] = "\0"; //gcc 실행 구문
    char run_syntax[30] = "\0"; //exe 실행 구문
    char stdoutfile_name[30] = "\0"; //stdout 파일 이름
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
            except_dot++ ; // .이나 ..은 생략한다
            continue;
        }
        strcpy(total_score_tab_for[idx].student, namelist[idx] -> d_name);
        printf("%s,",total_score_tab_for[idx].student); //학생들의 총 점수를 출력해준다.
        
    }

    for(idx = except_dot; idx < std_count; idx++) {
            strcpy(total_score_tab_for [idx - except_dot].student, total_score_tab_for[idx].student); //index 2부터 시작하므로 2칸씩 땡겨온다.
    }

    fd_backup = dup(1);
    dup2(fd_total , 1);

    printf(","); // 테이블 처음 칸은 빈칸
    for(idx = 0; idx < pro_count; idx++) {
        printf("%s,", ssu_score_tab_for[idx].name); //문제번호 컬럼
    }
    printf("Sum\n"); //총점수 컬럼
    for(idx = 0; idx < std_count-except_dot; idx++) {
        
        total_sum = 0;
        printf("%s,",total_score_tab_for[idx].student); // 학번 row

        for(jdx = 0; jdx < pro_count ; jdx++){
        total_score_tab_for[idx].score[jdx] = ssu_score_tab_for[jdx].score; //점수 row
        printf("%.2lf", total_score_tab_for[idx].score[jdx]);
        printf(",");
        total_sum += total_score_tab_for[idx].score[jdx]; //항목들을 더해가는 중
        }
        print_sum_count[idx] = total_sum; //총점수 row
        printf("%.2lf\n", total_sum);
        
    }

   
    
    dup2(fd_backup, 1);
    close(fd_total);

}

void score_table_create(char *directory_path_std, char *directory_path_ans, int print_flag) {

    struct dirent **namelist;
    int std_count, except_dot = 0; //except_dot : .이나 ..이 나오면 생략을 위한 카운팅 변수
    int idx, jdx;
    int fd_total, fd_backup, fd_backup2;
    double  total_sum, avg = 0; //총점, 평균점수
    double  print_sum_count[20];
    double  print_sum_count_total = 0;
    char exe_syntax[30] = "\0"; //exe 구문
    char gcc_syntax[30] = "\0"; //gcc 구문
    char run_syntax[30] = "\0"; //exe 실행 구문
    char stdoutfile_name[30] = "\0"; //stdout 이름용 배열
    char buf[BUFFER_SIZE];

    
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
            except_dot++ ; // .이나 ..이 나오면 생략시킨다
            continue;
        }
        
    }
        

    fd_backup = dup(1);
    dup2(fd_total , 1);

    printf(","); //처음 칸은 빈칸으로 놔둔다
    for(idx = 0; idx < pro_count; idx++) {
        printf("%s,", ssu_score_tab_for[idx].name); //문제 번호 컬럼
    }
    printf("Sum\n"); //sum 컬럼

    for(idx = 0; idx < std_count-except_dot; idx++) {
        
        total_sum = 0;
        printf("%s,",total_score_tab_for[idx].student); //학번 칸

        for(jdx = 0; jdx < pro_count ; jdx++){
        printf("%.2lf", total_score_tab_for[idx].score[jdx]); //score row
        printf(",");
        total_sum += total_score_tab_for[idx].score[jdx];
        }
        print_sum_count[idx] = total_sum;
        printf("%.2lf\n", total_sum); //총합 점수
        
    }
    
    dup2(fd_backup, 1);
    close(fd_total);
    

    if(print_flag == 1) { //p 옵션 사용시
        for(idx = 0 ; idx < std_count - except_dot ;idx++) {
    
        printf("%s score : %.2lf\n", total_score_tab_for[idx].student, print_sum_count[idx]);
        print_sum_count_total += print_sum_count[idx]; //각 학생들의 총합을 화면으로 출력

        }

        avg = print_sum_count_total / (std_count - except_dot);
        printf("Total Average : %.2lf\n", avg); //총 평균점수도 화면으로 출력
    }

}

