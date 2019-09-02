#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

#define STUDENT_NUM 9
#define PADDING_LEN 8

/* 0 : 48
 * 1 : 49
 * 2 : 50
 * 3 : 51
 * 4 : 52
 * 5 : 53
 * 6 : 54
 * 7 : 55
 * 8 : 56
 * 9 : 57*/
char padding[PADDING_LEN + 1] = {"00000000"};
char *test_id[STUDENT_NUM] = {"37", "73", "58", "85", "46",
  						 	  "34", "84", "33", "29"};
//char *test_id[STUDENT_NUM] = {"05", "15", "25", "39", "49", 
//							  "51", "62", "71", "84", "94",};

int main(int argc, char *argv[])
{
	int fd;
	char record_buf[STUDENT_RECORD_SIZE] = {0};

	/* Create student.dat */
	if ((fd = open(RECORD_FILE_NAME, O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)
	{
		fprintf(stderr, "Can't create %s file\n", RECORD_FILE_NAME);
		exit(1);
	}

	memset(record_buf, 0xff, sizeof(record_buf));
	memcpy(record_buf, padding, PADDING_LEN);

	/* Write  student */
	for (int i = 0; i < STUDENT_NUM; i++)
	{
		// set test id
		memcpy(record_buf + PADDING_LEN, test_id[i], 2);

		if (write(fd, record_buf, STUDENT_RECORD_SIZE) != STUDENT_RECORD_SIZE)
		{
			fprintf(stderr, "write() error\n");
			exit(1);
		}
	}

	close(fd);
	exit(0);
}
