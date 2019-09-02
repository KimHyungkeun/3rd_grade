#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define NAMESIZE 50

struct employee {
	char name[NAMESIZE];
	int salary;
	int pid;
};

int main(int argc, char* argv[]) {

	struct flock lock;
	struct employee record;
	int fd, recnum, pid;
	long position;
	char ans[5];

	if ((fd = open(argv[1], O_RDWR)) == -1) {
		perror(argv[1]);
		exit(1);
	}

	pid = getpid(); //프로세스 아이디 받아오기
	for(;;) {
		printf("\nEnter record number : ");
		scanf("%d", &recnum); //레코드 수 입력
		if(recnum < 0)
			break;

		position = recnum * sizeof(record); //위치지정
		lock.l_type = F_RDLCK; //타프로세스는 읽기 불가
		lock.l_whence = 0;
		lock.l_start = position;
		lock.l_len = sizeof(record);

		if(fcntl(fd, F_SETLKW, &lock) == -1) {
			perror(argv[1]); //다른프로세스가 이미 락을 설정 했을 경우 락을 해제할때까지 기다림
			exit(2);
		}

		lseek(fd, position, 0);
		if (read(fd, (char*)&record, sizeof(record)) == 0) {
			printf("record %d not found\n", recnum); //레코드 번호를 찾지 못할시 에러 출력
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			continue;
		}
		printf("Employee :%s, salary :%d\n", record.name,record.salary ); //사원이름과 급여 출력
		printf("Do you want to update salary (y or n)? ");
		scanf("%s", ans);

		if(ans[0] != 'y') { //갱신을 원하면 y입력
			lock.l_type = F_UNLCK;
			fcntl(fd, F_SETLK, &lock);
			continue;
		}
		lock.l_type = F_WRLCK; //타 프로세스는 읽기와 쓰기 락  모두 불가
		if(fcntl(fd, F_SETLKW, &lock) == -1) {
			perror(argv[1]);
			exit(3);
		}
		record.pid = pid; //프로세스 아이디 입력 받아오기
		printf("Enter new salary : ");
		scanf("%d", &record.salary); //급여 입력

		lseek(fd, position, 0);
		write(fd, (char*)&record, sizeof(record));//레코드 기록

		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
	}

	close(fd);
}
