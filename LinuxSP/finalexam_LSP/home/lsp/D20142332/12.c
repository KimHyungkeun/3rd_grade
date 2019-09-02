#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main (int argc, char *argv[]) {

	int fdin, fdout;
	void *src, *dat;
	struct stat statbuf;

	if (argc != 3) {
		printf("usage : %s <fromfile> <tofile>", argv[0]);
		exit(1);
	}

	fdin = open(argv[1], O_RDWR);
	fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);

	stat(argv[1], &statbuf);
	read(fdin, (char *)src, statbuf.st_size);
	write(fdout, (char *)src, statbuf.st_size);

	exit(0);
}
