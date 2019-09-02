#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SHM_MEM_SIZ 128
#define BUF_SIZ 128

void ssu_signal_handler1(int signo);
void ssu_signal_handler2(int signo);

char *data;

int main(void) {

	key_t key;
	pid_t pid[2];
	char buf[BUF_SIZ];
	int shm_id;
	int n;

	key = ftok("sem.c", 'T');
	shm_id = shmget(key, SHM_MEM_SIZ, 0644 | IPC_CREAT);
	data = shmat(shm_id, (void *) 0, 0);
	pid[0] = fork();

	if (pid[0] == 0) {
		if (signal(SIGUSR1, ssu_signal_handler1) == SIG_ERR) {
			fprintf(stderr, "signal error\n");
			exit(1);
		}

		while(1)
			sleep(1);
	}


	pid[1] = fork();
	if (pid[1] == 0) {

		if (signal(SIGUSR1, ssu_signal_handler2) == SIG_ERR) {
			fprintf(stderr, "signal error\n");
			exit(1);
		}

		while(1)
			sleep(1);
	}

	else {
		data[SHM_MEM_SIZ-1] = 0;
		while(1) {
			write(0,"parent : ", 8);
			n = read(1 , buf, BUF_SIZ);
			buf[n-1] = 0;
			sprintf(data, "%s", buf);
			kill(pid[0], SIGUSR1);
			kill(pid[1], SIGUSR1);
			if (strcmp(buf, "exit") == 0) {
				kill(pid[0], SIGKILL);
				kill(pid[1], SIGKILL);
				break;
			}
			while(data[SHM_MEM_SIZ-1] < 2);
			data[SHM_MEM_SIZ-1] = 0;
		}

		exit(0);

	}
}

void ssu_signal_handler1(int signo) {
	printf("child 1 : %s\n", data);
	data[SHM_MEM_SIZ-1]++;
}

void ssu_signal_handler2(int signo) {
	printf("child 2 : %s\n", data);
	data[SHM_MEM_SIZ-1]++;
}
