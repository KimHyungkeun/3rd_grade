#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(void) {

time_t first, second;
int error_fd, stdout_fd, printf_fd;
char gcc_arr[30];
char exe_arr[30];
char *ptr = "ANS_DIR/11/11.c";
char *ptr2 = "ANS_DIR/11/11.exe";

printf_fd = 1;
//sprintf(gcc_arr, "%s %s %s %s %s", "gcc", ptr, "-lpthread" ,"-o", ptr2);
/*error_fd = open("error12.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(error_fd,2);*/

sprintf(gcc_arr, "%s %s %s %s", "gcc", ptr, "-o", ptr2);
system(gcc_arr);

sprintf(exe_arr, "%s%s","./",ptr2);
stdout_fd = open("11.stdout", O_WRONLY | O_CREAT | O_TRUNC, 0644);
printf_fd = dup(1);

dup2(stdout_fd,1);
first = time(NULL);
system(exe_arr);
second = time(NULL);

close(stdout_fd);

exit(0);

}