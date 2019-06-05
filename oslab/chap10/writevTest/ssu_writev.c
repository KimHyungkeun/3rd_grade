#include <stdio.h>
#include <string.h>
#include <sys/uio.h>

int main(int argc, char **argv) {

	struct iovec vec[2];
	char MSG1[] = "0123456789";
	char MSG2[] = "abcdefghijklmnopqrstuvwxyz";
	int str_len;

	vec[0].iov_base = MSG1; //MSG1 배열 저장
	vec[0].iov_len = strlen(MSG1); //MSG1의 길이
	vec[1].iov_base = MSG2; //MSG2 배열 저장
	vec[1].iov_len = strlen(MSG2); //MSG2의 길이

	str_len = writev(1, vec, 2); //총 길이 출력

	printf("\n총 %d 바이트 출력\n", str_len);

	return 0;
}
