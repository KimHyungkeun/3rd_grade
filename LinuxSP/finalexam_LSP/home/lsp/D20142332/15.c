#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 256

int main(int argc, char *argv[]) {

	FILE *fp;
	int length = 0;
	int cnt = 0;
	char strJava[BUFSIZE];

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "fopen error\n");
		exit(1);
	}

		fseek(fp ,0 , SEEK_END);
		length = ftell(fp);
	

	printf("%s file size if %d bytes\n", argv[1], length);

	
	fread(strJava, BUFSIZE, 1, fp);
	strcpy(strJava, "\0");
	fseek(fp , 0 ,SEEK_SET);

	while(!feof(fp)) {
		
		fgets(strJava,sizeof(strJava) , fp);
		cnt++;
	}

	fseek(fp, 0, SEEK_SET);

	cnt--;
	fclose(fp);

	printf("%s line number is %d lines\n", argv[1], cnt);

	return 0;
}
