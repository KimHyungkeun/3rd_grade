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

	if ((fd = open(argv[1], O_RDWR)) == -1) {
		perror(argv[1]);
		exit(1);
	}

	pid = getpid();
	for(;;) {

		printf("\nEnter record number : ");
		scanf("%d", &recnum); //레코드 수 입력
		if (recnum < 0)
			break;
		position = recnum * sizeof(record); //레코드 위치
		lock.l_type = F_WRLCK; //다른프로세스는 읽기 락과 쓰기 락 모두 불가
		lock.l_whence = 0;
		lock.l_start = position;
		lock.l_len = sizeof(record); //레코드 길이
		if(fcntl(fd, F_SETLKW, &lock) == -1) { //다른프로세스가 이미 락을 설정하면 학을 해제할때까지 대기
			perror(argv[1]); 
			exit(2);
		}
		lseek(fd, position, 0); //해당 위치까지 오프셋 이동
		if (read(fd, (char*)&record, sizeof(record)) == 0) {
			printf("record %d not found\n", recnum); //파일 읽기 실패시 에러메세지 출력
			lock.l_type = F_UNLCK; //학 해제
			fcntl(fd, F_SETLK, &lock);//이미 락 설정이면 -1 리턴
			continue;
		}
		printf("Employee : %s, salary : %d\n", record.name, record.salary);
		record.pid = pid; //프로세스 아이디 대입
		printf("Enter new salary : ");
		scanf("%d", &record.salary);
		lseek(fd, position, 0);
		write(fd, (char *)&record, sizeof(record));//레코드 쓰기

		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
	}
	close(fd);

}


