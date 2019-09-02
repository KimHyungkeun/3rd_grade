#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	
	pid_t pgid;
	pid_t pid;

	if (argc < 2) {
		fprintf(stderr, "usage : %s <file>\n",argv[0]);
		exit(1);
	}

	pid = getpid(); //process id
	pgid = getpgid(atoi(argv[1])); //process group id
	printf("pid: %d, pgid : %d\n", pid, pgid); //print ids.
	exit(0);
}
