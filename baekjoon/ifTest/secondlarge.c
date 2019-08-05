#include <stdio.h>
#include <stdlib.h>

int main() {

   int num[3];
   int large_num;
   int second_num;
   int count = 0;
   scanf("%d %d %d", &num[0], &num[1], &num[2]);
   
    if (num[0] < 1 || num[1] < 1 || num[2] < 1 || num[0] > 100 || num[1] > 100 || num[2] > 100) {

       while(1) {
        scanf("%d %d %d", &num[0], &num[1], &num[2]);
        if ((1 <= num[0] && num[0] <= 100) && (1 <= num[1] && num[1] <= 100) && (1 <= num[2] && num[2] <= 100))
            break;
       }
    }

    for (int i = 0 ; i < 3 ; i++) {
        for (int j = 0 ; j < 3 ; j++) {
     
            if (num[i] >= num[j])
                count++;

            if (count == 3) {
                large_num = num[i];
                break;
            }
        }

        if (count == 3)
            break;

        count = 0;
    }
   
    

    for (int i = 0 ; i < 3 ; i++) {
        if (num[i] == large_num) {
            num[i] = -50;
            break;
        }
    }

    count = 0;
    for (int i = 0 ; i < 3 ; i++) {
        for (int j = 0 ; j < 3 ; j++) {
     
            if (num[i] >= num[j])
                count++;

            if (count == 3) {
                second_num = num[i];
                break;
            }
        }

        if (count == 3)
            break;

        count = 0;
    }

    printf("%d\n", second_num);
    exit(0);
}