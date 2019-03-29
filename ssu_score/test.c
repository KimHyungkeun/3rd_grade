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

int convert_string(const void *a, const void *b)
{
    return strcmp((char *)a, (char *)b);
}

int main()
{
   char s[10][8];
    
    for(int i=0;i<10;i++){
       scanf("%s",s[i]);
   }

    qsort(s, sizeof(s) / sizeof(s[0]), sizeof(s[0]), convert_string);

    for (int i = 0; i < 10; i++)
    {
        printf("%s ", s[i]);
    }

    printf("\n");

    return 0;
}