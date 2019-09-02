#include <stdio.h>
#include <stdlib.h>

int main() {

   int num1, num2;
   scanf("%d %d", &num1, &num2);

   if (num1 < 1 || num2 < 1 || num1 > 10000 || num2 > 10000) {

       while(1) {
        scanf("%d %d", &num1, &num2);
        if ((1 <= num1 && num1 <= 10000) && (1 <= num2 && num2 <= 10000))
            break;
       }
   }

   printf("%d\n", num1 + num2);
   printf("%d\n", num1 - num2);
   printf("%d\n", num1 * num2);
   printf("%d\n", num1 / num2);
   printf("%d\n", num1 % num2);

   exit(0);
}