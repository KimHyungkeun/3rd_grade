#include "ssu_score.h"


void ssu_runtime (struct timeval* begin_t, struct timeval* end_t) {  //프로그램 수행 시간을 측정하기 위한 함수

	end_t -> tv_sec -= begin_t -> tv_sec;

	if(end_t -> tv_usec < begin_t -> tv_usec) {
		end_t -> tv_sec--;
		end_t -> tv_usec += SECOND_TO_MICRO;
	}

	end_t -> tv_usec -= begin_t -> tv_usec;
	printf("Runtime : %ld:%06ld(sec:usec)\n",end_t -> tv_sec, end_t -> tv_usec);

}

void directory_check(char** argv) { //디렉터리 내부 파일 및 서브디렉터리를 확인하기 위한 함수


   DIR            *dir_info, *ans_dir; //dir_info : 처음 부모디렉터리, ans_dir : ANS_DIR 내부의 디렉터리
   struct dirent  *dir_entry, *ans_entry; // dir_info를 readdir 하기 위한 변수, ans_dir를 readdir 하기 위한 변수
   const char *sub_directory; // sub_directory 이름을 담는 변수
   char directory_path[20] = "\0"; //디렉터리 경로 이름
   char directory_path_backup[20] = "\0"; //파일 열기 전 path로 돌아간다.
   char buf_ANS[BUFFER_SIZE], buf_STD[BUFFER_SIZE]; //ANS 의 파일내용을 담기 위한 변수, STD의 파일내용을 담기 위한 변수
   int fd, count; //fd : 파일 디스크립터, count : 읽은 buf의 갯수

	
	
   strcat(argv[1],"/"); //make format like "ANS_DIR/"
   strcat(directory_path, argv[1]); //directory path = "ANS_DIR/"

   dir_info = opendir(argv[1]); // 현재 디렉토리를 열기

  if ( NULL != dir_info)
   {
      while ( dir_entry   = readdir( dir_info)) {   // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
	strcat(dir_entry -> d_name,"/"); //make format like "SUBDIR/"
  	printf( "Subdirectory : %s ", dir_entry -> d_name); 
	strcat(directory_path, dir_entry -> d_name); //make format like "ANS_DIR/SUBDIR/"
	printf( "PATH : %s\n ",directory_path); 
		
  	ans_dir = opendir(directory_path); //open directory "ANS_DIR/SUBDIR/"

		while (ans_entry = readdir( ans_dir)) {	
			strcpy(directory_path_backup, directory_path); 
			printf("%s ",ans_entry -> d_name); //filelist of SUBDIR
			strcat(directory_path, ans_entry -> d_name); //make format like "ANS_DIR/SUBDIR/[Filename]"			

				fd = open(directory_path,O_RDONLY); //open *.txt
				if( fd < 0) {
				fprintf(stderr, "open error for %s\n",directory_path);
				exit(1);
				}	

			count = read(fd, buf_ANS, BUFFER_SIZE); //처음부터 시작하여 1024byte 만큼을 읽는다.
			buf_ANS[count] = 0;
			printf("fd's printf : \n%s\n",buf_ANS); //읽은 문자열을 출력
	
		strcpy(directory_path, directory_path_backup);	
		close(fd);
		}

	printf("\n");
        closedir(ans_dir);  	//ANS의 Subdirectory를 닫는다.
	strcpy(directory_path,argv[1]);	//direcotrypath를 다시 "ANS_DIR/"로 바꾼다.
	}

      printf("\n");	
      closedir( dir_info); // 가장 부모가되는 디렉터리를 닫는다.

   }

} 


int main(int argc, char *argv[])
{

	
   struct timeval begin_t, end_t; //시작시간과 끝나는 시간	
   gettimeofday(&begin_t, NULL);

   if(argc < 2) {
	fprintf(stderr,"Usage : %s ANS_DIR\n",argv[0]);
	gettimeofday(&end_t,NULL);
       	ssu_runtime(&begin_t, &end_t);
	exit(1);
	}
	
	directory_check(argv);//direcotory check	
	gettimeofday(&end_t,NULL);
       	ssu_runtime(&begin_t, &end_t);
	exit(0);
}


