#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int ssu_write(int fd, char* buf);

int main(void) {

	char buf[128];
	char pattern[4] = "bcd";
	char *pos1 = buf, *pos2 = buf;
	char *fname_in = "in.txt";
	char *fname_out = "out.txt";
	int size;
	int fd1, fd2;
	int i = 0;

	if((fd1 = open(fname_in, O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", fname_in);
		exit(1);
	}

	size = read(fd1, pos1, sizeof(buf));
	close(fd1);

	if((fd2 = open(fname_out, O_RDWR | O_CREAT | O_TRUNC , 0644)) < 0) {
		fprintf(stderr, "open error for %s\n", fname_out);
		exit(1);
	}

	for( ; i < size ; i++) {
		if(buf[i] == pattern[0] && buf[i+1] == pattern[1] && buf[i+2] == pattern[2]) {
			buf[i] = '0';
			buf[i+1] = '0';
			buf[i+2] = '0';
		}
	}

	ssu_write(fd2, pos2);
	close(fd2);

	return 0;

}

int ssu_write(int fd, char* buf) {

	
	return write(fd, buf, strlen(buf));

}
