#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COUNT 10

int compare(const void *a, const void *b);

int main(void) {
    
    int i;
    char arr[MAX_COUNT][7] = {
        "1-1", "10-1", "10-2", "10-3", "10-4", 
        "10-5", "90-1","9-2","9-3","9-4"
    };

    qsort(arr, MAX_COUNT, sizeof(arr[0]), compare);

    printf("\n");
    for(i=0;i<MAX_COUNT;i++) {
        printf("%s\n", arr[i]);
    }
    printf("\n");

    return 0;
}

int compare(const void *a, const void *b) {
    return strcmp((char*)a, (char*)b);
}
