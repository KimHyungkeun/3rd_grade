#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define NAMESIZE 50
#define MAXTRIES 5

struct employee {

	char name[NAMESIZE];
	int salary;
	int pid;
};

int main(int argc, char *argv[]) {

	struct flock lock;
	struct employee record;
	int fd, sum = 0, try = 0;

	sleep(10);

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror(argv[1]); //파일을 연다. 없으면 에러처리
		exit(1);
	}

	lock.l_type = F_RDLCK; // 읽기 락만 가능
	lock.l_whence = 0; 
	lock.l_start = 0L; // 바이트 단위 오프셋
	lock.l_len = 0L; // EOF를 잠근다

	while (fcntl(fd, F_SETLK, &lock) == -1) {
		if (errno == EACCES) { 

			if(try++ < MAXTRIES) { //최대횟수까지 try를 증가
				sleep(1);
				continue;
			}
			printf("%s busy == try later\n", argv[1]); 
			exit(2);
		}
		perror(argv[1]); //파일에러시 종료
		exit(3);
	}

	while(read(fd, (char *)&record, sizeof(record)) > 0) {
		printf("Employee : %s, Salary : %d\n", record.name, record.salary);
		sum += record.salary; //급여 증가
	}
	printf("\nTotal salary : %d\n", sum); //총 급여 출력

	lock.l_type = F_UNLCK; //잠금해제
	fcntl(fd, F_SETLK, &lock); //락 설정
	close(fd);
}
