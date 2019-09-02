#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {

	char buf[BUFFER_SIZE];
        int fd, fd2;
        int length;
	int length_argv;
	int i;
	int offset = atoi(argv[2]);

        if(argc != 4) {
                fprintf(stderr,"Usage : %s file offset data\n",argv[0]);
                exit(1);
        }

        if((fd = open(argv[1],O_RDWR)) < 0) {
                fprintf(stderr,"open error for %s\n",argv[1]);
                exit(1);
        }

	if((fd2 = creat("temp.txt",0666)) < 0) {
		fprintf(stderr,"open error for %s\n","temp.txt");
		exit(1);
	}

	
	if(lseek(fd, offset, SEEK_SET) < 0) {
		fprintf(stderr,"lseek error\n");
		exit(1);
	}

	while ( (length = read(fd, buf, BUFFER_SIZE)) > 0) {
		write(fd2, buf, length);
	}
	
	length_argv = strlen(argv[3]);	

	/*for(i = length-1 ; i >= offset ; i--) {

		if(length_argv < 10) {

		buf[i+length_argv] = buf[i];
		buf[i] = '\0';

		}

		else {
		buf[i+10] = buf[i];
		buf[i] = '\0';
		
		}
	}*/

	if(lseek(fd, offset, SEEK_SET) < 0) {
		fprintf(stderr, "lseek error\n");
		exit(1);
	}
		

	if( length_argv  < 10) {
		write(fd, argv[3], length_argv);
		lseek(fd, offset + length_argv, SEEK_SET);
	}

	else {
        	write(fd,argv[3],10);
		lseek(fd, offset + 10, SEEK_SET);
	}
	//write(fd,buf,length + length_argv);

	printf("%s\n", buf);
	
	while ((length = read(fd2, buf, BUFFER_SIZE)) > 0) {
		write(fd, buf, length);	
	}
	
	printf("%s\n",buf);
	close(fd2);

	remove("temp.txt");
        exit(0);



}

