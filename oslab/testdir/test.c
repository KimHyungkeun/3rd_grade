#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
	
	char *str1 = "qwer";
	char str2[10] = "abcde"; 
	printf("str1 before : %s\n",str1);
	strcpy(str2,str1); 
	printf("str1 : %s\n",str1);

	strcat(str2,str1);
	printf("str1 : %s\n",str1);
	printf("str2 : %s\n",str2);


	strcat(str2,str1);
	
	printf("str1 : %s\n",str1);
	printf("str2 : %s\n",str2);
	exit(0);
}
