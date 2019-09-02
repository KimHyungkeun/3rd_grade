#include <stdio.h>
#include <stdlib.h>

int main() {

    int i;
    int num;
    scanf("%d", &num);

    if (num < 1 || num > 9) {
        while(1) {
            scanf("%d", &num);
            if (1 <= num && num <= 9)
                break;
        }
    }

    for (i=1;i<=9;i++) {
        printf("%d * %d = %d\n", num,i,num*i);
    }
    exit(0);
}