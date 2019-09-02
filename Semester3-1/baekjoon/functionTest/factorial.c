#include <stdio.h>
#include <stdlib.h>

int main() {

    int i,n;
    int mul = 1;
    scanf("%d", &n);

    if (n < 0 || n > 12) {
        while(1) {
            scanf("%d", &n);

            if (0 <= n && n <= 12) {
                break;
            }
        }
    }

    for (i=n;i>0;i--) {
        mul *= i;
    }

    printf("%d\n", mul);
    exit(0);
}

