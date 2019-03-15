#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>



int main(int argc, char *argv[])
{

   DIR            *dir_info, *ans_dir;
   struct dirent  *dir_entry, *ans_entry;
   const char *sub_directory;
   char directory_path[20] = "\0";


   if(argc < 2) {
	fprintf(stderr,"Usage : %s \n",argv[0]);
	exit(1);
	}
	
   strcat(argv[1],"/"); //make format like "DIR/"
   strcat(directory_path, argv[1]); //directory path

   dir_info = opendir(argv[1]); // 현재 디렉토리를 열기

  if ( NULL != dir_info)
   {
      while ( dir_entry   = readdir( dir_info)) {   // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
  	printf( "Subdirectory : %s ", dir_entry -> d_name);
	strcat(directory_path, dir_entry -> d_name);
	printf( "PATH : %s\n ",directory_path);
	
	
  	ans_dir = opendir(directory_path);
	while (ans_entry = readdir( ans_dir)) {
	printf("%s ",ans_entry -> d_name);

	}
	printf("\n");
        closedir(ans_dir);  	
	strcpy(directory_path,argv[1]);	
	}

      printf("\n");	
      closedir( dir_info);

   }
	exit(0);
}

