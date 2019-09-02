#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


int main(int argc, char* argv[]) {

	struct stat statbuf;
	ino_t inode;

	if(argc != 3) {
		fprintf(stderr, "argc != 3\n");
		exit(1);
	}

	lstat(argv[1], &statbuf);	
	printf("%s's inode = %ld -> ", argv[1], inode = statbuf.st_ino);

	if(link(argv[1], argv[2]) == -1) {
		fprintf(stderr, "link error\n");
		exit(1);
	}

	lstat(argv[2], &statbuf);
	printf("%s's inode = %ld\n", argv[2], inode = statbuf.st_ino);

	if(unlink(argv[1]) == -1) {
		fprintf(stderr, "unlink error\n");
	}



	exit(0);
}
