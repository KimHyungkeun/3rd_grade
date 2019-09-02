#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

   int num, count = 0;
   int result, new_result;
   char temp_num[3];
   char new_temp[3];
   char last_temp[3];
   temp_num[2] = '\0';
   new_temp[2] = '\0';
   last_temp[2] = '\0';

   scanf("%d", &num);

    if ( num < 0 || num > 99 ) {
        while(1) {
        scanf("%d", &num);
        if ((0 <= num && num <= 99))
            break;
        }
    }

    sprintf(temp_num, "%d", num);

    if (strlen(temp_num) == 1) {
        temp_num[1] = temp_num[0];
        temp_num[0] = '0';
    }

    while(1) {

        count++;
        result = (temp_num[0] - 48) + (temp_num[1] - 48); 

        sprintf(new_temp, "%d", result);
        if (strlen(new_temp) == 1) {
            new_temp[1] = new_temp[0];
            new_temp[0] = '0';
            }
        
        last_temp[0] = temp_num[1];
        last_temp[1] = new_temp[1];
        
        
        if (atoi(last_temp) == num)
            break;

        else {
            sprintf(temp_num, "%d", atoi(last_temp));

            if (strlen(temp_num) == 1) {
            temp_num[1] = temp_num[0];
            temp_num[0] = '0';
            }
        }

        
    }

    printf("%d\n", count);
    
    exit(0);
}