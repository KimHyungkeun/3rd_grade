#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main (void) {

    /*char arr[30];
    sprintf(arr, "%s %s", "Hello", "World!");
    printf("%s\n", arr);
    sprintf(arr, "%s", "Apple");
    printf("%s\n", arr);
    sprintf(arr, "%s", "D");
    printf("%s\n", arr);*/

    char *ptr;
    ptr = strpbrk("10-5.txt", ".txt");
    printf("%s\n", ptr);

    exit(0);

}