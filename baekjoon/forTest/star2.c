#include <stdio.h>
#include <stdlib.h>

int main() {

    int i,j;
    int num;
    scanf("%d", &num);

    if (num < 1 || num > 100) {
        while(1) {
            scanf("%d", &num);
            if (1 <= num && num <= 100)
                break;
        }
    }

    for (i=1;i<=num;i++) {
        for(j=0;j<num-i;j++){
            printf(" ");
        }

        for(;j<num;j++) {
            printf("*");
        }
        printf("\n");
    }
    exit(0);
}