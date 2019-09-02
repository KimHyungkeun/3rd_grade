#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {

    int result[1000];
    int n1,n2;
    int count = 0;
    int i;
    
    while(1) {
        scanf("%d %d", &n1,&n2);

        if (n1 == 0 && n2 == 0)
            break;

        if (n1 < 1 || n2 < 1 || n1 > 9 || n2 > 9) {
            while(1) {
            scanf("%d %d", &n1,&n2);
            if ((1 <= n1 && n1 <= 9) && (1 <= n2 && n2 <= 9))
                break;
            }
        }

    result[count] = n1 + n2;
    count++;
    }

    for (i=0;i<count;i++)
        printf("%d\n", result[i]);
      
    exit(0);
}