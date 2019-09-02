#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

void putRC(char *chr, int times)
{
    char *ptr = chr + strlen(chr);
    while (times--) {
        if (random() % 3 == 0)
            *ptr = 'A' + (random() % 26);
        else {
            if (random() % 2)
                *ptr = 'a' + (random() % 26);
            else
                *ptr = '0' + (random() % 10);
        }
        ptr++;
    }
}

int main(void) {

    FILE *dat_fp = fopen(RECORD_FILE_NAME, "w+");
    char packbuf[STUDENT_RECORD_SIZE] = {0, };
    char *ptr = packbuf;
    int i;
    int n;
    int dat_record;

    printf("만들 개수 입력 : ");
    scanf("%d", &dat_record);
  

	for (i = 0; i < dat_record; ++i) {
        	printf("Input StudentID : ");
        	scanf("%10s", ptr);
        	ptr = &packbuf[10];
        	putRC(ptr, 20);
        	ptr = &packbuf[30];
        	putRC(ptr, 30);
        	ptr = &packbuf[60];
        	putRC(ptr, 1);
        	ptr = &packbuf[61];
        	putRC(ptr, 19);
        	ptr = &packbuf[80];
        	putRC(ptr, 15);
        	ptr = &packbuf[95];
        	putRC(ptr, 25);
        	ptr = &packbuf[120];
        	fwrite(packbuf, STUDENT_RECORD_SIZE, 1 ,dat_fp);
        	memset(packbuf, 0, STUDENT_RECORD_SIZE);
        	ptr = packbuf;
    	}
    
		fclose(dat_fp); 

   
}