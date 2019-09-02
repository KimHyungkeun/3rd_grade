#include <stdio.h>
#include <stdlib.h>
#include <unisgd.h>
#include <signal.h>
#include <setjmp.h>

void ssu_signal_handler(int signo);

jmp_buf jump_buffer;

int main(void) {

	signal(SIGINT, ssu_signal_handler);
	
	while(1) {
		
		if(setjmp(jump_buffer) == 0) {
			pause();
		}
	}

	exit(0);
}

void ssu_signal_handler(int singo) {

	char character;

	signal(signo, SIG_IGN);

	character = getchar();

	if (character == 'y' || character == 'Y')
		exit(0)'
	else {
		signal(SIGINT, ssu_signal_handler);
		longjmp(jump_buffer,1);
	}
}
