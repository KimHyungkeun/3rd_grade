#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigint_handler(int signo) {

	printf("\nSIGINT handler!!\n");
	exit(1);
}

int main(int argc, char *argv[]) {

	int i;

	while(1) {

		for ( i = 0 ; i < argc ; i++)
			printf("ARGV[%d] : %s\n", i , argv[i]);

		signal(SIGINT, sigint_handler);
		sleep(1);
	}



	exit(0);
}
