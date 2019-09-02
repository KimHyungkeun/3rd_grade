#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

/*int main (void) {

    system("ls -v ANS_DIR > score.csv");
    exit(0);
    
}*/

int main()
{
   int num;
   printf("Select mode : ");
   scanf("%d\n", &num);

   while(1) {
       
       if(num != 4) {
       printf("Retry : ");
       scanf("%d", &num);
       }
       
       else
           break;
       
       
   }
}