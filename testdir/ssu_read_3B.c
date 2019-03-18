#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(void) {
	
	char *fname = "ssu_test.txt";
	int fd, line_count = 0;
	char character;

	if((fd = open(fname, O_RDONLY)) < 0) {
		fprintf(stderr,"open error for %s\n",fname);
		exit(1);
	}

	while(1) {	
		if(read(fd,&character,1) > 0) {
			if(character == '\n')
				line_count++;
		}
		
		else
			break;

	}
	printf("Total Line : %d",line_count);
	exit(0);
		
}

