#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define PAGES_PER_BLOCK       4   // 상수값 수정 가능
#define RS_BUF_PAGES            0.3 // 여기서 '0.3은 30%를 의미하며 상수값 수정 가능
#define BUF_PAGES_PER_BLOCK (int)PAGES_PER_BLOCK*RS_BUF_PAGES
#define NONBUF_PAGES_PER_BLOCK  (PAGES_PER_BLOCK - BUF_PAGES_PER_BLOCK)

int main(void) {

    float buf_pages_per_block = PAGES_PER_BLOCK * RS_BUF_PAGES ;
    int nonbuf_pages_per_block = PAGES_PER_BLOCK - BUF_PAGES_PER_BLOCK;

    printf("Buf_pages_per_block : %d\n", (int)buf_pages_per_block);
    printf("Nonbuf_pages_per_block : %d\n", nonbuf_pages_per_block);

    exit(0);

}