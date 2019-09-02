#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {

    int *result;
    int n1,n2;
    int num;
    int i;
    scanf("%d", &num);

    result = (int*)malloc(sizeof(int) * num);
    for (i=0;i<num;i++) {
        scanf("%d %d", &n1,&n2);
        if (n1 < 1 || n2 < 1 || n1 > 10 || n2 > 10) {
            while(1) {
            scanf("%d %d", &n1,&n2);
            if ((1 <= n1 && n1 <= 10) && (1 <= n2 && n2 <= 10))
                break;
            }
        }
        result[i] = n1 + n2;
    }

    for (i=0;i<num;i++) {
        printf("Case #%d: %d\n", i+1, result[i]);
    }
    
    free(result);
    exit(0);
}