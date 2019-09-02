#include <sys/time.h>

#define SECOND_TO_MICRO 1000000

void ssu_runtime (struct timeval* begin_t, struct timeval* end_t) {  //프로그램 수행 시간을 측정하기 위한 함수

        long begin;
        long end;
        end = end_t -> tv_usec + (end_t -> tv_sec * SECOND_TO_MICRO);
        begin = begin_t -> tv_usec + (begin_t -> tv_sec * SECOND_TO_MICRO);

        printf("%ld us\n",end - begin);
        
}

