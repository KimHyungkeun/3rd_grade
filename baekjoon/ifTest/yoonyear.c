#include <stdio.h>
#include <stdlib.h>

int main() {

    int year;
    scanf("%d", &year);

    if (year > 4000 || year < 1) {
       while(1) {
        scanf("%d", &year);
        if (1 <= year && year <= 4000)
            break;
       }
    }

    if (year % 4 == 0) {
        if (year % 100 != 0)
            printf("1\n");

        else if (year % 400 == 0)
            printf("1\n");

        else 
            printf("0\n");
    }

    else 
        printf("0\n");

    exit(0);
}