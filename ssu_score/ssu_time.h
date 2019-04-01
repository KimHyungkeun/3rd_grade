#include <sys/time.h>

#define SECOND_TO_MICRO 1000000

void ssu_runtime (struct timeval* begin_t, struct timeval* end_t) {  //프로그램 수행 시간을 측정하기 위한 함수

        end_t -> tv_sec -= begin_t -> tv_sec; //끝나는 시간에서 시작 시간을 하나씩 뺀다

        if(end_t -> tv_usec < begin_t -> tv_usec) { //만약 시작시간이 더 큰경우에는
                end_t -> tv_sec--;
                end_t -> tv_usec += SECOND_TO_MICRO; //tv_sec을 하나 빼고 1000000큼을 더해준다 (받아올림 받아내림의 개념)
        }

        end_t -> tv_usec -= begin_t -> tv_usec;
        printf("Runtime : %ld:%06ld(sec:usec)\n",end_t -> tv_sec, end_t -> tv_usec);  //총 걸린 시간 출력

}

