#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal(int signo) {

	printf("SIGUSR1 caughted\n");
}

int main(void) {

	pid_t pid;
	sigset_t sig_set;
	sigset_t pending_sigset;

	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sig_set, NULL);

	signal(SIGUSR1, ssu_signal);
	kill(getpid(), SIGUSR1);

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid == 0) {
		sigpending(&pending_sigset);

			if(sigismember(&pending_sigset, SIGUSR1))
				printf("child : SIGUSR1 pending\n");
	}

	else {
		sigpending(&pending_sigset);

			if(sigismember(&pending_sigset, SIGUSR1))
				printf("parent : SIGUSR1 pending\n");
	}
}
