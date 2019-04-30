#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void print_file_type(struct stat *statbuf) {
	char *str;

	if(S_ISREG(statbuf -> st_mode))
		str = "regular";
	else if (S_ISDIR(statbuf -> st_mode))
		str = "directory";
	else if (S_ISCHR(statbuf -> st_mode))
		str = "Character";
	else if (S_ISBLK(statbuf -> st_mode))
		str = "Block file";
	else if (S_ISFIFO(statbuf -> st_mode))
		str = "FIFO file";
	else if (S_ISLNK(statbuf -> st_mode))
		str = "Symbolic link";
	else if (S_ISSOCK(statbuf -> st_mode))
		str = "Socket file";
	else
		str = "Unknown mode";

	printf("%s\n", str);
}

int main(void) {

	struct stat statbuf;
	char *fname = "symlink";
	
	if(lstat(fname, &statbuf) < 0) {
		fprintf(stderr, "stat error\n");
		exit(1);
	}

	print_file_type(&statbuf);

	exit(0);



}
