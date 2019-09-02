#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct _person {
	char name[10];
	int age;
	double height;
} Person;

int main(void) {

	FILE *fp;
	int i, res;
	Person ary[3] = {{"Hong GD", 500, 175.4}, {"Lee SS", 350, 180.0}, {"King SJ", 500, 178.6}};
	Person tmp;

	if((fp = fopen("ftest.txt", "wb")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", "ftest.txt");
		exit(1);
	}

	fwrite(ary, sizeof(ary), 1, fp);
	fclose(fp);
	

	if((fp = fopen("ftest.txt", "rb")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", "ftest.txt");
		exit(1);
	}

	printf("[ First print]\n");
	while(!feof(fp)) {
		if(res = fread(&tmp, sizeof(Person), 1 ,fp) != 1)
			break;
		printf("%s %d %.2lf\n", tmp.name, tmp.age, tmp.height);
	}

	fseek(fp, 0 , SEEK_SET);

	printf("[ Second print]\n");

	while(!feof(fp)) {
		if(res = fread(&tmp, sizeof(Person), 1 ,fp) != 1)
			break;
		printf("%s %d %.2lf\n", tmp.name, tmp.age, tmp.height);
	}

	fclose(fp);

	exit(0);
}
