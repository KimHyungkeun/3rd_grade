#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "record.h"
//필요하면 header file 추가 가능

//
// argv[1]: 레코드 파일 students.dat에 저장되는 전체 레코드의 수
//

#define RECORD_SIZE 250

int main(int argc, char **argv)
{
    int count;
    int fd;
    Student student;

    if(argc < 2) {
        fprintf(stderr, "Usage : %s number\n", argv[0]);
        exit(1);
    }

    if((fd = open("students.dat", O_WRONLY | O_CREAT | O_TRUNC, 0640)) < 0) {
        fprintf(stderr, "open error for %s\n", "students.dat");
        exit(1);
    }

    count = atoi(argv[1]);

    for(int idx = 0; idx < count ; idx++) {
        strncpy(student.id, "2019000111", 10);
        strncpy(student.name, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCD", 30);
        strncpy(student.address, "Wolgye, Nowon-gu, Seoul, Korea, Asia, Earth, SolarSystem, Universe,END",70);
        strncpy(student.univ, "SoongsilUniversitySoongsilUniv", 30);
        strncpy(student.dept, "ComputerScienceComputerScienceComputerSc", 40);
        strncpy(student.others, "HelloWorldHelloWorldHelloWorldHelloWorldHelloWorldHelloWorldHelloWorld", 70);
        write(fd, (char*)&student, RECORD_SIZE);
    }

    close(fd);
    exit(0);


    // 250바이트 학생 레코드를 students.dat 파일에 하나씩 저장하는 코드를 구현함
    // 학생 레코드의 데이터는 임의로 생성해도 무방함
    
	return 0;
}
