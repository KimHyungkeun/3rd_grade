#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

	struct stat file_info;
	int i;
	char* str;

	for(i=1;i<argc;i++) {
		printf("name = %s type = ",argv[i]);

		if(lstat(argv[i],&file_info) < 0) {
			fprintf(stderr,"lstat error\n");
			exit(1);
		}


		if(S_ISREG(file_info.st_mode))
			str = "Regular";
		else if(S_ISDIR(file_info.st_mode))
			str = "Directory";
		else if(S_ISCHR(file_info.st_mode))
			str = "Character";
		else if(S_ISBLK(file_info.st_mode))
			str = "Block special";
		else if(S_ISLNK(file_info.st_mode))
			str = "Symbolic link";
		else if(S_ISFIFO(file_info.st_mode))
			str = "FIFO file";
		else if(S_ISSOCK(file_info.st_mode))
			str = "Socket file";
		else
			str = "Unknown mode";

		printf("%s\n", str);
	}

	exit(0);
}

