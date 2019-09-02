#include <stdio.h>
#include <stdlib.h>

int main() {

    int i;
    int num;
    scanf("%d", &num);

    if (num < 1 || num > 100000) {
        while(1) {
            scanf("%d", &num);
            if (1 <= num && num <= 100000)
                break;
        }
    }

    for (i=num;i>=1;i--) {
        printf("%d\n", i);
    }
    
    exit(0);
}