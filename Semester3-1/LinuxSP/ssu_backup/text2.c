#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {

    char buf[50];
    realpath("dir.txt", buf);

    printf("buf : %s\n", buf);

    return 0;
}
