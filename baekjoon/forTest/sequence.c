#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main(void) {

    int i;
    int n,x;
    int* sequence;
    scanf("%d %d", &n, &x);
    if (n < 1 || x < 1 || n > 10000 || x > 10000) {
            while(1) {
            scanf("%d %d", &n,&x);
            if ((1 <= n && n <= 10000) && (1 <= x && x <= 10000))
                break;
            }
    }

    sequence = (int*)malloc(sizeof(int) * n);
    for(i=0;i<n;i++) {
        scanf("%d ", &sequence[i]);
    }

    free(sequence);
    exit(0);
}