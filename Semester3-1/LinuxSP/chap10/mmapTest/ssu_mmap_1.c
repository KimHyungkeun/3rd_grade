#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char *argv[]) {

	int fdin, fdout;
	void *src, *dst;
	struct stat statbuf;

	if (argc != 3) {
		printf("usage : %s <fromfile> <tofile>", argv[0]);
		exit(1); //인자가 3개가 아니면 에러처리
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0) {
		printf("Can't open %s for reading", argv[1]);
		exit(1); //명령어 다음 첫번째 인자에 넣은 파일에 대해 에러 발생시 에러처리후 종료
	}

	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
		printf("Can't open %s for reading", argv[1]);
		exit(1); //명령어 다음 첫번째 인자에 넣은 파일에 대해 에러 발생시 에러처리후 종료
	}

	if (fstat(fdin, &statbuf) < 0) { /*입력 파일의 크기*/
		printf("fstat() error");
		exit(1);
	}

	/* 출력 파일의 크기 */
	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1) {
		printf("lseek() error"); //파일에 이상이 있으면 에러처리후 종료
		exit(1);
	}

	if (write(fdout, "", 1) != 1) {
		printf("write() error"); //쓰기 에러시 종료
		exit(1);
	}

	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED) {

		printf("mmap() error for input");
		exit(1); //소스파일에 대한 영역읽기 및 저장 연산이 매핑된 파일을 수정. 
	}

	if ((dst = mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED) {

		printf("mmap() error for output");
		exit(1); //목적지파일에 대한 영역읽기와 쓰기 및 저장 연산이 매핑된 파일을 수정
	}

	memcpy(dst, src, statbuf.st_size);
	exit(0);


}
