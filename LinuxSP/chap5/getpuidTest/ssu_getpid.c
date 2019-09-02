#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "ssu_time.h"

int main(void) {

	struct timeval begin, end;
	gettimeofday(&begin, NULL);

	printf("Process ID	= %d\n", getpid()); //호출한 프로세스 ID
	printf("Parent process ID	= %d\n", getppid()); // 부모 프로세스 ID
	printf("Real user ID	= %d\n", getuid()); //실제 사용자 ID
	printf("Effective user ID	= %d\n", geteuid()); // 유효 사용자 ID
	printf("Real group ID	= %d\n", getgid()); //실제 그룹 ID
	printf("Effective ID	= %d\n", getegid()); // 유효 그룹 ID
	
	gettimeofday(&end, NULL);
	ssu_runtime(&begin, &end);
	exit(0);
}
