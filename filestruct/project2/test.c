#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {

    char name[11] = "HelloWorld\0";
    printf("%ld\n", sizeof(name));
    printf("%s\n", name);
    printf("%ld\n", strlen(name));
    exit(0);
}
