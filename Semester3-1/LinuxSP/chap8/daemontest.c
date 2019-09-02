#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>

int ssu_daemon_init(void);

int main(void) {

	pid_t pid;

	pid = getpid();
	printf("parent process %d is running\n", pid);
	printf("daemon process initialization\n");

	if (ssu_daemon_init() < 0) {
		fprintf(stderr, "daemon initialization is failed\n");
		exit(1);
	}

	exit(0);
}

int ssu_daemon_init(void) {

	pid_t pid;
	int fd, maxfd;

	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid != 0)
		exit(0);

	pid = getpid();
	printf("daemon process %d is running\n", pid);
	setsid();
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	maxfd = getdtablesize();

	for (fd = 0 ; fd < maxfd ; fd++)
		close(fd);

	umask(0);
	chdir("/");

	fd = open("/dev/null", O_RDWR);

	dup(0);
	dup(0);

	return 0;
}
