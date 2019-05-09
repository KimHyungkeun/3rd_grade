#include <stdio.h>
#include <string.h>
#include "blkmap.h"

extern FILE *devicefp;				// main.c에 정의되어 있음
extern int write_cnt;
extern int erase_cnt;

int dd_read(int ppn, char *pagebuf)
{
	int ret;

	fseek(devicefp, PAGE_SIZE*ppn, SEEK_SET);
	ret = fread((void *)pagebuf, PAGE_SIZE, 1, devicefp);
	if(ret == 1) {
		return 1;
	}
	else {
		return -1;
	}
}

int dd_write(int ppn, char *pagebuf)
{
	int ret;
	write_cnt++;

	fseek(devicefp, PAGE_SIZE*ppn, SEEK_SET);
	ret = fwrite((void *)pagebuf, PAGE_SIZE, 1, devicefp);
	if(ret == 1) {			
		return 1;
	}
	else {
		return -1;
	}
}

int dd_erase(int pbn)
{
	char blockbuf[BLOCK_SIZE];
	int ret;

	erase_cnt++;

	memset((void*)blockbuf, (char)0xFF, BLOCK_SIZE);
	
	fseek(devicefp, BLOCK_SIZE*pbn, SEEK_SET);
	
	ret = fwrite((void *)blockbuf, BLOCK_SIZE, 1, devicefp);
	
	if(ret == 1) { 
		return 1;
	}
	else {
		return -1;
	}
}
