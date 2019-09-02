#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>

static void ssu_alarm(int signo);
static void ssu_func(int signo);
void ssu_mask(const char *str);

static volatile sig_atomic_t can_jump;
static sigjmp_buf jump_buf;

int main(void) {

	if (signal(SIGUSR1, ssu_func) == SIG_ERR)

	if (signal(SIGALRM, ssu_alarm) == SIG_ERR)

	ssu_mask("starging main :");

	if (sigsetjmp(jump_buf, 1)) 

	can_jump = 1;

	while(1)
		pause();

	exit(0);

}

void ssu_mask(const char *str) {

	sigset_t sig_set;
	int err_num;

	err_num = errno;

	if (sigprocmask(0, NULL, &sig_set) < 0) {
		exit(1);
	}

	if (sigismember(&sig_set, SIGINT))

	if (sigismember(&sig_set, SIGQUIT))
	
	if (sigismember(&sig_set, SIGUSR1))

	if (sigismember(&sig_set, SIGALRM))

	printf("\n");
	errno = err_num;
}

static void ssu_func(int signo) {

	time_t start_time;

	if (can_jump == 0)
		return;

	ssu_mask("starting ssu_func : ");
	alarm(3);
	start_time = time(NULL);

	while(1)
		if(time(NULL) > start_time + 5)
			break;

	ssu_mask("endging ssu_func :");
	can_jump = 0;
	siglongjmp(jump_buf, 1);
}

static void ssu_alarm(int signo) {
	ssu_mask("in ssu_alarm :");
}
