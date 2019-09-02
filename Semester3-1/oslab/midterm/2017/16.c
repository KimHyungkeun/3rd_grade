#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
	struct stat statbuf;
	char *str;
	int i;

	for(i=1;i<argc;i++) {
		printf("name = %s, type = ",argv[i]);

		if(lstat(argv[1], &statbuf) < 0) {
			fprintf(stderr, "lstat error\n");
			continue;
		}

		if(S_ISREG(statbuf.st_mode))
			str = "regular";
		else if(S_ISDIR(statbuf.st_mode))
			str = "Directory";
		else if(S_ISCHR(statbuf.st_mode))
			str = "Chracter special";
		else if(S_ISBLK(statbuf.st_mode))
			str = "Block special";
		else if(S_ISFIFO(statbuf.st_mode))
			str = "FIFO file";
		else if(S_ISLNK(statbuf.st_mode))
			str = "symbolic link";
		else if(S_ISSOCK(statbuf.st_mode))
			str = "socket file";
		else
			str = "unknown mode";

		printf("%s\n",str);
	}

	exit(0);
}


