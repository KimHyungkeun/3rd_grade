#include <stdio.h>
#include <stdlib.h>

struct ssu_id {

	char name[64];
	int id;
};

int main(void) {

	struct ssu_id test1, test2;
	char *fname = "ssu_exam.dat";
	FILE *fp;
	

	if((fp = fopen(fname, "w")) == NULL) { //파일을 쓰기모드로 오픈
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	printf("Input ID>> ");
	scanf("%d", &test1.id);
	printf("Input name>> ");
	scanf("%s", test1.name);

	if(fwrite(&test1, sizeof(struct ssu_id), 1, fp) != 1) {
		fprintf(stderr, "fwrite error\n"); //fwrite 에러시 에러출력
		exit(1);
	}	
	

	fclose(fp);

	if((fp = fopen(fname, "r")) == NULL) { //파일을 읽기모드로 오픈 
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	if(fread(&test2, sizeof(struct ssu_id), 1, fp) != 1) {
		fprintf(stderr, "fread error\n"); //fread 에러시 에러출력
		exit(1);
	}
	printf("\nID     name\n");
	printf(" ============\n");
	printf("%d       %s\n", test2.id, test2.name);
	fclose(fp);
	
	exit(0);
}






