#include <stdio.h>
#include <stdlib.h>

int main() {

    int i;
    int num;
    int sum = 0;
    scanf("%d", &num);

    if (num < 1 || num > 10000) {
        while(1) {
            scanf("%d", &num);
            if (1 <= num && num <= 10000)
                break;
        }
    }

    for (i=1;i<=num;i++) {
        sum += i;
    }

    printf("%d\n", sum);
    exit(0);
}