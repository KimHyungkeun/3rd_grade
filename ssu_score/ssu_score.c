#include "ssu_score.h"
#include "ssu_time.h"

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
	
	blank_problem_check(directory_path_std, directory_path_ans); //빈칸 채우기 문제 	
	program_problem_check(directory_path_std, directory_path_ans); //프로그래밍 문제
	score_table_create(directory_path_ans);

	gettimeofday(&end_t,NULL);
       	ssu_runtime(&begin_t, &end_t);
	exit(0);

}

char *txt_search(char *str) {
	
	char *ptr;
	ptr = strstr(str,".txt");
        printf("%s\n",ptr);	
	return ptr;	

}

char *c_search(char *str) {

	char *ptr;
	ptr = strstr(str,".c");
	printf("%s\n",ptr);
	return ptr;
}

void blank_problem_check(char* directory_path_std, char* directory_path_ans) {


   DIR *dir_info_ans, *dir_sub_ans, *dir_info_std, *dir_sub_std; //dir_info : 처음 부모디렉터리, dir_* : DIR_* 내부의 디렉터리
   struct dirent  *dir_entry_ans, *entry_sub_ans, *dir_entry_std, *entry_sub_std; // dir_info_*를 readdir 하기 위한 변수, dir_*를 readdir 하기 위한 변수
   const char *sub_directory_ans; // ANS_DIR의 sub_directory 이름을 담는 변수
   const char *sub_directory_std; // STD_DIR의 sub_directory 이름을 담는 변수
   char directory_root_ans[20] = "\0"; //ANS_DIR  
   char directory_root_std[20] = "\0"; //STD_DIR
   char directory_path_backup_ans[20] = "\0"; // ANS_DIR/SUBDIR/로 돌아감
   char directory_path_backup_std[20] = "\0"; // STD_DIR/SUBDIR/로 돌아감
   char buf_ANS[BUFFER_SIZE]; // ANS 의 파일내용을 담기 위한 변수
   char buf_STD[BUFFER_SIZE]; // STD의 파일내용을 담기 위한 변수
   int fd_ans, fd_std, count; //fd : 파일 디스크립터, count : 읽은 buf의 갯수
		

    strcpy(directory_root_ans,directory_path_ans);
    strcpy(directory_root_std,directory_path_std); 

   dir_info_std = opendir(directory_root_std); // STD_DIR 열기
   //dir_info_ans = opendir(argv[2]); // ANS_DIR 열기
	

  if ( NULL != dir_info_std)
   {
      while ( dir_entry_std   = readdir( dir_info_std)) {   // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
	strcat(dir_entry_std -> d_name,"/"); //make format like "SUBDIR/"
  	printf( "Subdirectory : %s ", dir_entry_std -> d_name); 
	strcat(directory_path_std, dir_entry_std -> d_name); //make format like "STD_DIR/SUBDIR/"
	printf( "PATH : %s\n ",directory_path_std); 
		
  	dir_sub_std = opendir(directory_path_std); //open directory "STD_DIR/SUBDIR/"

		while (dir_entry_std = readdir( dir_sub_std)) {	
			strcpy(directory_path_backup_std, directory_path_std); 
			printf("Read %s ",dir_entry_std -> d_name); //filelist of SUBDIR
			strcat(directory_path_std, dir_entry_std -> d_name); //make format like "STD_DIR/SUBDIR/[Filename]
				//printf("%d\n",strcmp(c_search(directory_path_std),".c"));
				if(strcmp(dir_entry_std -> d_name,".") == 0 || strcmp(dir_entry_std -> d_name,"..") == 0 )  {
					//해당 디렉터리 내에 '.' 이나 '..'같은 것이 있으면 건너뛴다.	
					strcpy(directory_path_std, directory_path_backup_std);	
				}
	
				else {
					fd_std = open(directory_path_std,O_RDONLY); //open *.txt
					if( fd_std < 0) {
					fprintf(stderr, "open error for %s\n",directory_path_std);
					exit(1);
					}

					count = read(fd_std, buf_STD, BUFFER_SIZE); //처음부터 시작하여 1024byte 만큼을 읽는다.
					buf_STD[count] = 0;
					printf("fd's printf : \n%s\n",buf_STD); //읽은 문자열을 출력
					
					strcpy(directory_path_std, directory_path_backup_std);	
					close(fd_std);
				}	

	
		}

	printf("\n");
        closedir(dir_sub_std);  //STD의 Subdirectory를 닫는다.
	printf("%s\n",directory_path_std);
	strcpy(directory_path_std,directory_root_std); //direcotrypath를 다시 "STD_DIR/"로 바꾼다.
	printf("%s\n",directory_path_std);
	}

      printf("\n");	
      closedir( dir_info_std); // 가장 부모가되는 디렉터리를 닫는다.

   }
}

 
void program_problem_check(char* directory_path_std, char* directory_path_ans) {


   DIR *dir_info_ans, *dir_sub_ans, *dir_info_std, *dir_sub_std; //dir_info : 처음 부모디렉터리, dir_* : DIR_* 내부의 디렉터리
   struct dirent  *dir_entry_ans, *entry_sub_ans, *dir_entry_std, *entry_sub_std; // dir_info_*를 readdir 하기 위한 변수, dir_*를 readdir 하기 위한 변수
   const char *sub_directory_ans; // ANS_DIR의 sub_directory 이름을 담는 변수
   const char *sub_directory_std; // STD_DIR의 sub_directory 이름을 담는 변수
   char directory_root_ans[20] = "\0"; //ANS_DIR  
   char directory_root_std[20] = "\0"; //STD_DIR
   char directory_path_backup_ans[20] = "\0"; // ANS_DIR/SUBDIR/로 돌아감
   char directory_path_backup_std[20] = "\0"; // STD_DIR/SUBDIR/로 돌아감
   char buf_ANS[BUFFER_SIZE]; // ANS 의 파일내용을 담기 위한 변수
   char buf_STD[BUFFER_SIZE]; // STD의 파일내용을 담기 위한 변수
   int fd_ans, fd_std, count; //fd : 파일 디스크립터, count : 읽은 buf의 갯수

    strcpy(directory_root_ans,directory_path_ans);
    strcpy(directory_root_std,directory_path_std); 
   //dir_info_std = opendir(directory_path_std); // STD_DIR 열기
   dir_info_ans = opendir(directory_path_ans); // ANS_DIR 열기
	

  if ( NULL != dir_info_ans)
   {
      while ( dir_entry_ans   = readdir( dir_info_ans)) {   // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
	strcat(dir_entry_ans -> d_name,"/"); //make format like "SUBDIR/"
  	printf( "Subdirectory : %s ", dir_entry_ans -> d_name); 
	strcat(directory_path_ans, dir_entry_ans -> d_name); //make format like "ANS_DIR/SUBDIR/"
	printf( "PATH : %s\n ",directory_path_ans); 
		
  	dir_sub_ans = opendir(directory_path_ans); //open directory "ANS_DIR/SUBDIR/"

		while (dir_entry_ans = readdir( dir_sub_ans)) {	
			strcpy(directory_path_backup_ans, directory_path_ans); 
			printf("Read %s ",dir_entry_ans -> d_name); //filelist of SUBDIR
			strcat(directory_path_ans, dir_entry_ans -> d_name); //make format like "ANS_DIR/SUBDIR/[Filename]
				//printf("%d\n",strcmp(txt_search(directory_path_ans),".txt"));				
				if(strcmp(dir_entry_ans -> d_name,".") == 0 || strcmp(dir_entry_ans -> d_name,"..") == 0 )  {
                                         //해당 디렉터리 내에 '.' 이나 '..'같은 것이 있으면 건너뛴다.
                                         strcpy(directory_path_ans, directory_path_backup_ans);
                                 } 
				
				else{		
					fd_ans = open(directory_path_ans,O_RDONLY); //open *.c
					if( fd_ans < 0) {
					fprintf(stderr, "open error for %s\n",directory_path_ans);
					exit(1);
					}

					count = read(fd_ans, buf_ANS, BUFFER_SIZE); //처음부터 시작하여 1024byte 만큼을 읽는다.
					buf_ANS[count] = 0;
					printf("fd's printf : \n%s\n",buf_ANS); //읽은 문자열을 출력
					
					strcpy(directory_path_ans, directory_path_backup_ans);	
					close(fd_ans);
			   	}
	
		}

	printf("\n");
        closedir(dir_sub_ans);  //ANS의 Subdirectory를 닫는다.
	printf("%s\n",directory_path_ans);
	strcpy(directory_path_ans,directory_root_ans); //direcotrypath를 다시 "ANS_DIR/"로 바꾼다.
	printf("%s\n",directory_path_ans);
	}

      printf("\n");	
      closedir( dir_info_ans); // 가장 부모가되는 디렉터리를 닫는다.

   }

}

void score_table_create(char* directory_path_ans) {

	struct table_format tab_for; //table format
 	int fd_score, count=0; //fd : 파일 디스크립터, count : 읽은 buf의 갯수
	tab_for.comma = ',';
	tab_for.line_jump = '\n';



  if((fd_score = open("score_table.csv",O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
	  fprintf(stderr,"open error for %s\n","score_table.csv");
	  exit(1);
  }	

  while(1) {
	  
	  if(count == 49)
	  break;

	  printf("Input of ");
	  printf("%s : ",filename[count]);
	  scanf("%d",&tab_for.score);
	  strcpy(tab_for.name, filename[count]);
	  write(fd_score, (char *)&tab_for, sizeof(tab_for));

	  count ++;
	  
  }

  close(fd_score);
  exit(0);

}

