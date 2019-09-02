#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int main() {

    int *result;
    int n1,n2;
    int num;
    int i;

    scanf("%d", &num);
    if (num < 1 || num > 1000000 ) {
            while(1) {
            scanf("%d", &num);
            if ((1 <= num && num <= 1000000))
                break;
            }
        }

    result = (int*)malloc(sizeof(int) * num);
    for (i=0;i<num;i++) {
        scanf("%d %d", &n1,&n2);
        if (n1 < 1 || n2 < 1 || n1 > 1000 || n2 > 1000) {
            while(1) {
            scanf("%d %d", &n1,&n2);
            if ((1 <= n1 && n1 <= 1000) && (1 <= n2 && n2 <= 1000))
                break;
            }
        }
        result[i] = n1 + n2;
    }

    for (i=0;i<num;i++) {
        printf("%d\n", result[i]);
    }
    
    free(result);

    exit(0);
}