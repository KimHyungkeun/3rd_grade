#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {

    char *fname = "ssu_new.txt";
    int fd;

    if((fd = creat(fname, 0666)) < 0) {
        fprintf(stderr, "creat error for %s\n", fname);
        exit(1);
    }

    printf("First printf is on the screen.\n");
    dup2(fd,1);
    printf("Second printf is in this file.\n");
    exit(0);
}