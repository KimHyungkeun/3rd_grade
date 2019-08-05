#include <stdio.h>
#include <stdlib.h>

int main() {

   int num1, num2;
   scanf("%d %d", &num1, &num2);

   if (num1 < -10000 || num2 < -10000 || num1 > 10000 || num2 > 10000) {

       while(1) {
        scanf("%d %d", &num1, &num2);
        if ((-10000 <= num1 && num1 <= 10000) && (-10000 <= num2 && num2 <= 10000))
            break;
       }
   }

    if (num1 > num2)
        printf(">\n");

    else if (num1 < num2)
        printf("<\n");

    else if (num1 == num2)
        printf("==\n");

    exit(0);
}