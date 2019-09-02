#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void print_file_type(struct stat *statbuf) {
	char *str;

	if(S_ISREG(statbuf -> st_mode))
		str = "Regular";
	else if(S_ISDIR(statbuf -> st_mode))
		str = "Directory";
	else if(S_ISCHR(statbuf -> st_mode))
		str = "character special";
	else if(S_ISBLK(statbuf -> st_mode))
		str = "block speical";
	else if(S_ISFIFO(statbuf -> st_mode))
		str = "FIFO";
	else if(S_ISSOCK(statbuf -> st_mode))
		str = "socket";
	else if(S_ISLNK(statbuf -> st_mode))
		str = "symbolic link";
	else
		str = "unknown mode";

	printf("%s\n", str);
}

int main(int argc, char* argv[]) {
	
	int i;
	struct stat statbuf;

    for(i = 0 ; i < argc ; i++) {
		if(lstat(argv[i], &statbuf) < 0) {
			fprintf(stderr, "error\n");
			continue;
		}	


	print_file_type(&statbuf);

	}


	exit(0);
	
}
