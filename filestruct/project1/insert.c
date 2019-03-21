#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 2048

int main(int argc, char* argv[]) {

	char buf[BUFFER_SIZE];
        int fd;
        int length;
	int length_argv;
	int i;
	off_t offset = atoi(argv[2]);

        if(argc != 4) {
                fprintf(stderr,"Usage : %s file offset data\n",argv[0]);
                exit(1);
        }

        if((fd = open(argv[1],O_RDWR)) < 0) {
                fprintf(stderr,"open error for %s\n",argv[1]);
                exit(1);
        }


	length = read(fd, buf, BUFFER_SIZE); //원본파일의 모든 글자를 읽은 크기
	length_argv = strlen(argv[3]); // 입력한 문자열의 길이

	for(i = length-1 ; i >= offset ; i--) { //지정한 offset을 기준으로, 입력한 문자열 길이만큼 뒤로 민다.
		
		if( length_argv < 10) {
			buf[i+length_argv] = buf[i];
			buf[i] = '\0';
		}

		else {
			buf[i+10] = buf[i];
			buf[i] = '\0';
		}
		
	}

	lseek(fd, 0, SEEK_SET); //offset을 처음으로 지정

	if( length_argv < 10 )
		write(fd, buf, length + length_argv); //밀었던 만큼 공백을 발생시킴

	else
		write(fd,buf,length+10);

	lseek(fd, offset, SEEK_SET);

	if( length_argv < 10)
        	write(fd, argv[3], length_argv); //해당 공백에 새로 insert할 문자열이 써진다.

	else
		write(fd, argv[3], 10);

        exit(0);



}
