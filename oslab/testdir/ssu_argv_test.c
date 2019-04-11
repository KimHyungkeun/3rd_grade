#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void argv_print(char** );

int main(int argc, char* argv[]) {

	if(argc != 3) {
		fprintf(stderr,"Usage : ssu_argv_test file1 file2\n");
		exit(1);
	}

	argv_print(argv);

	exit(0);
}


void argv_print(char** argv) {

	printf("%s\n", argv[0]);
	printf("%s\n", argv[1]);
	printf("%s\n", argv[2]);

}

