#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLEN 256

int main(int argc, char *argv[]) {

	char *fname, *makefile;
	char *tempStr;
	int fd;

	if (argc != 2) {
		fprintf(stderr, "Usage : %s filename\n", argv[0]);
		exit(1);
	}

	fname = argv[1];

	makefile = malloc(sizeof(char) * MAXLEN);
	tempStr = malloc(sizeof(char) * MAXLEN);

	memset(makefile, 0, sizeof(makefile));
	memset(tempStr, 0 , sizeof(tempStr));

	sprintf(makefile , "%s%s%s", strtok(fname, "."),"_" ,"Makefile");

	if ((fd = open(makefile, O_WRONLY| O_CREAT | O_TRUNC, 0644)) < 0) {
		fprintf(stderr, "open error\n");
		exit(1);
	}

	sprintf(tempStr,"%s %s %s%s\n",strtok(fname, "."), ":" ,strtok(fname,"."), ".o");
	write(fd, tempStr, strlen(tempStr));
	memset(tempStr, 0 , sizeof(tempStr));

	sprintf(tempStr,"\t %s %s %s %s%s\n","gcc", "-o", strtok(fname, "."), strtok(fname, "."), ".o");
	write(fd, tempStr, strlen(tempStr));
	memset(tempStr, 0 , sizeof(tempStr));

	sprintf(tempStr,"%s%s %s %s%s\n",strtok(fname, "."), ".o",":" ,strtok(fname,"."), ".c");
	write(fd, tempStr, strlen(tempStr));
	memset(tempStr, 0 , sizeof(tempStr));

	sprintf(tempStr,"\t %s %s %s %s%s %s%s\n","gcc", "-c","-o", strtok(fname, "."),".o", strtok(fname, "."), ".c");
	write(fd, tempStr, strlen(tempStr));
	memset(tempStr, 0 , sizeof(tempStr));


	sprintf(tempStr,"%s\n","clean :");
	write(fd, tempStr, strlen(tempStr));
	memset(tempStr, 0 , sizeof(tempStr));

	sprintf(tempStr, "\t %s %s\n", "rm", strtok(fname, "."));
	write(fd, tempStr, strlen(tempStr));
	memset(tempStr, 0 , sizeof(tempStr));

	close(fd);
	printf("%s make success\n", makefile);
	return 0;
}
