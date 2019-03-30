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
   char *str = "This is has error: find somethng";
   char *ptr;
   ptr = strstr(str, "warning");

   if(ptr == NULL)
   printf("Nothing here\n");
   
   else
   printf("%s\n", ptr);

    return 0;
}