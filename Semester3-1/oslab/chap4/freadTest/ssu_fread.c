#include <stdio.h>
#include <stdlib.h>
#include "ssu_time.h"

struct ssu_pirate {
	unsigned long booty;
	unsigned int beard_length;
	char name[128];
};

int main(void) {

	struct timeval begin_t, end_t;
	struct ssu_pirate blackbeard = {950, 48, "Edward Teach"} , pirate;
	char *fname = "ssu_data";
	FILE *fp1, *fp2;

	gettimeofday(&begin_t, NULL);

	if((fp2 = fopen(fname, "w")) == NULL) { //쓰기모드로 오픈
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}	

	if(fwrite(&blackbeard, sizeof(struct ssu_pirate),1, fp2) != 1) {
		fprintf(stderr, "fwrite error\n"); //파일에 내용 적기
		exit(1);
	}

	if (fclose(fp2)) { //파일 닫음
		fprintf(stderr, "fclose error\n");
		exit(1);
	}

	if((fp1 = fopen(fname, "r")) == NULL) { //파일을 읽기모드로 오픈
		fprintf(stderr, "fopen error\n");
		exit(1);
	}

	if(fread(&pirate, sizeof(struct ssu_pirate),1, fp1) != 1) {
		fprintf(stderr, "fread error\n"); //파일을 읽음
		exit(1);
	}

	if(fclose(fp1)) { //파일 닫음
		fprintf(stderr, "fclose error\n");
		exit(1);
	}

	printf("name=\"%s\" booty=%lu beard_length=%u\n", pirate.name, pirate.booty, pirate.beard_length);

	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t); //프로그램 수행시간 측정
	exit(0);

}
