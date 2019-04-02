#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "record.h"
#include "ssu_time.h"

#define RECORD_SIZE 250
//필요하면 header file 추가 가능

//
// argv[1]: 레코드 파일명
// argv[2]: 레코드 파일에 저장되어 있는 전체 레코드의 수
//

int main(int argc, char **argv)
{
	Student student;
	struct stat statbuf;
	struct timeval begin_t, end_t;
	int length;
    int fd;
	int count = 0;

	gettimeofday(&begin_t, NULL);

    if(argc < 3) {
        fprintf(stderr, "Usage : %s filename records\n", argv[0]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
        exit(1);
    }

    if((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "open error for %s\n", argv[1]);
		gettimeofday(&end_t, NULL);
		ssu_runtime(&begin_t, &end_t);
        exit(1);
    }
	// 표준입력으로 받은 레코드 파일로부터 전체 레코드를 "순차적"으로 읽어들이고, 이때
	// 걸리는 시간을 측정하는 코드 구현함
	// 파일 i/o 시간 비용 측정 실험이므로 파일로부터 읽어 들인 레코드를 굳이 학생 구조체 변수에
    // 저장할 필요는 없음


	stat(argv[1], &statbuf);
	char buf[statbuf.st_size];

	while(1) {

		if((length = read(fd, buf, RECORD_SIZE)) <= 0) {
			break;
		}
	}
		
	close(fd);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	return 0;
}
