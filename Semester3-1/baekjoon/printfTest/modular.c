#include <stdio.h>
#include <stdlib.h>

int func1(int,int,int);
int func2(int,int,int);
int func3(int,int,int);
int func4(int,int,int);

int main() {

    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
   
    if (a < 2 || b < 2 || c < 2 || a > 10000 || b > 10000 || c > 10000) {

       while(1) {
        scanf("%d %d %d", &a, &b, &c);
        if ((2 <= a && a <= 10000) && (2 <= b && b <= 10000) && (2 <= c && c <= 10000))
            break;
       }
    }

    printf("%d\n", func1(a,b,c));
    printf("%d\n", func2(a,b,c));
    printf("%d\n", func3(a,b,c));
    printf("%d\n", func4(a,b,c));


    exit(0);
}

int func1(int a, int b, int c) {

    return (a+b)%c;
}

int func2(int a, int b, int c) {
    
    return (a%c + b%c)%c;
}

int func3(int a, int b, int c) {
    
    return (a*b)%c;
}

int func4(int a, int b, int c) {
    
    return ((a%c) * (b%c))%c;
}