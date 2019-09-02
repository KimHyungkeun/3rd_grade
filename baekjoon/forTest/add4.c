#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {

    int **result;
    int *result_sum;
    int n1,n2;
    int num;
    int i;
    scanf("%d", &num);

    result_sum = (int*)malloc(sizeof(int) * num);
    result = (int**)malloc(sizeof(int*) * num);
    for (i=0;i<num;i++) {
        result[i] = (int*)malloc(sizeof(int) * 2);
    }

    for (i=0;i<num;i++) {
        scanf("%d %d", &n1,&n2);
        if (n1 < 1 || n2 < 1 || n1 > 10 || n2 > 10) {
            while(1) {
            scanf("%d %d", &n1,&n2);
            if ((1 <= n1 && n1 <= 10) && (1 <= n2 && n2 <= 10))
                break;
            }
        }
        result[i][0] = n1;
        result[i][1] = n2;
        result_sum[i] = n1 + n2;
    }

    for (i=0;i<num;i++) {
        printf("Case #%d: %d + %d = %d\n", i+1, result[i][0], result[i][1], result_sum[i]);
    }
    
    free(result_sum);
    for(i=0;i<num;i++) {
        free(result[i]);
    }
    free(result);
    exit(0);
}