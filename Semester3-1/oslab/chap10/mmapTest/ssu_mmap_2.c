#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#ifndef MAP_FILE
#define MAP_FILE 0
#endif

void serror(char *str) {

	fprintf(stderr, "%s\n", str);
	exit(1);
}

int main(int argc, char *argv[]) {

	int fdin, fdout;
	char *inp, *outp;
	struct stat statbuf;

	if (argc != 3)
		serror("usage : a.out [source file] [target file]");

	if ((fdin = open(argv[1], O_RDONLY)) < 0)
		serror("can't open file for reading");//파일이 없거나 열기에 실패하면 에러처리

	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
		serror("can't creat file for reading"); //파일 생성에 실패하면 에러 처리

	if (fstat(fdin, &statbuf) < 0)
		serror("fstat error");

	/*출력 파일의 크기를 지정*/
	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1)
		serror("lseek error");

	if (write(fdout, " ", 1) != 1)
		serror("write error"); //쓰기 실패시 에러처리

	if ((inp = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t) -1)
		serror("mmap error for input"); //input파일에 대한 영역읽기 및 저장 연산이 매핑된 파일을 수정


	if ((outp = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t) -1)
		serror("mmap error for input"); //output파일에 대한 영역읽기와 쓰기 및 저장 연산이 매핑된 파일을 수정

	memcpy(outp, inp, statbuf.st_size);

	exit(0);
	
}
