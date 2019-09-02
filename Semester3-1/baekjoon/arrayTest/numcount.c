#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int main() {

    int a,b,c;
    int result;
    int i,j;
    int count[10]={0};
    char result_str[10];

    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &c);

    if (a < 100 || b < 100 || c < 100 || a > 999 || b > 999 || c > 999) {
        while (1) {
            scanf("%d", &a);
            scanf("%d", &b);
            scanf("%d", &c);

            if ((100 <= a && a <= 999) && (100 <= b && b <= 999) && (100 <= c && c <= 999))
                break;
        }
    }

    result = a * b * c;
    sprintf(result_str, "%d", result);
   

    for(i=0;i<=9;i++) {
        for(j=0;j<strlen(result_str);j++) {
            if(i == (result_str[j] - 48))
                count[i]++;
        }
    }

    for(i=0;i<10;i++) {
        printf("%d\n", count[i]);
    }

  
    exit(0);
}