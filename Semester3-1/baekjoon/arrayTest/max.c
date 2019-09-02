#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {

    int i,j;
    int max;
    int seq;
    int count=0;
    int num[9];

    for (i=0;i<9;i++) {
        scanf("%d", &num[i]);
    }

    for (i=0;i<9;i++) {
        for(j=0;j<9;j++) {
            if(num[i] >= num[j])
                count++;
            
            if (count == 9){
                max = num[i];
                seq = i;
                break;
            }
        }
        count = 0;
    }

    printf("%d\n%d\n", max, seq+1);

    exit(0);
}