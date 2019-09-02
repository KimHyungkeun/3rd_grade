#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void main(void) {

	char *fname = "ssu_line.txt";
	char *frname = "ssu_blank.txt";
	int fd_w, fd_r;
	size_t length;
	int wordcnt = 1;
	char buf[50];
	int i = 0;

	if((fd_w = open(fname, O_WRONLY | O_CREAT)) < 0) {
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);
	}


	if((fd_r= open(frname, O_RDONLY | O_CREAT)) < 0) {
		fprintf(stderr, "creat error for %s\n", frname);
		exit(1);
	}

	if((length = read(fd_r, buf, sizeof(buf))) < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}


	lseek(fd_r, 0 , SEEK_SET);

	for(i = 0 ; i < length ; i++) {
		if(buf[i] == ' ') {
			buf[i] = '\n';
			wordcnt++;
		}
	}

	printf("wordcount = %d\n", wordcnt);


	

	close(fd_r);


}
