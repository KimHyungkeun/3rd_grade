#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {

	struct flock lock;
	char *fname = "test.txt";
	int fd;
	pid_t pid;
	char c;

	if ((fd = open(fname, O_RDWR)) < 0) {
		fprintf(stderr, "open error\n");
		exit(1);
	}

	lock.l_type = F_WRLCK;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid == 0) {
		sleep(1);
		int i = 0;
		while(read(fd, &c, 1) > 0) {
			lock.l_whence = SEEK_SET;
			lock.l_start = i++;
			lock.l_len = 1;

			if (fcntl(fd, F_SETLKW, &lock) < 0) {
				fprintf(stderr, "fcntl error\n");
				exit(1);
			}
			fprintf(stderr, "%c", c);
		}

		exit(1);
	} 

	else {

			lock.l_whence = SEEK_END;
			lock.l_start = -6;
			lock.l_len = 0;

			
			if (fcntl(fd, F_SETLKW, &lock) < 0) {
				fprintf(stderr, "fcntl error\n");
				exit(1);
			}

			if (fcntl(fd, F_SETLK, &lock) < 0) {
				fprintf(stderr, "fcntl error\n");
				exit(1);
			}

			while(1);
		
	}

	exit(0);
}
