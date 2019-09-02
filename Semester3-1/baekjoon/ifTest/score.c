#include <stdio.h>
#include <stdlib.h>

int main() {

    int score;
    scanf("%d", &score);

    if (score > 100 || score < 0) {
       while(1) {
        scanf("%d", &score);
        if (0 <= score && score <= 100)
            break;
       }
    }

    if (90 <= score && score <= 100)
        printf("A\n");
    
    else if (80 <= score && score <= 89)
        printf("B\n");

    else if (70 <= score && score <= 79)
        printf("C\n");

    else if (60 <= score && score <= 69)
        printf("D\n");

    else
        printf("F\n");
    

    exit(0);
}