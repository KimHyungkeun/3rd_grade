#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "record.h"
#include "ssu_time.h"

#define RECORD_SIZE 250
//�ʿ��ϸ� header file �߰� ����

//
// argv[1]: ���ڵ� ���ϸ�
// argv[2]: ���ڵ� ���Ͽ� ����Ǿ� �ִ� ��ü ���ڵ��� ��
//
int main(int argc, char **argv)
{
	struct timeval begin_t, end_t;
	int length;
    int fd;
	int count = 0;
	long long size;
	char *buf;

	

    if(argc < 3) {
        fprintf(stderr, "Usage : %s filename records\n", argv[0]);
        exit(1);
    }

    if((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "open error for %s\n", argv[1]);
        exit(1);
    }
	// ǥ���Է����� ���� ���ڵ� ���Ϸκ��� ��ü ���ڵ带 "������"���� �о���̰�, �̶�
	// �ɸ��� �ð��� �����ϴ� �ڵ� ������
	// ���� i/o �ð� ��� ���� �����̹Ƿ� ���Ϸκ��� �о� ���� ���ڵ带 ���� �л� ����ü ������
    // ������ �ʿ�� ����

	size = atoi(argv[2]) * RECORD_SIZE; 
	buf = (char*)malloc(size);

	gettimeofday(&begin_t, NULL);
	while(1) {

		if((length = read(fd, buf, RECORD_SIZE)) <= 0) {
			break;
		}
		
	}
		
	close(fd);
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	free(buf);
	return 0;
}
