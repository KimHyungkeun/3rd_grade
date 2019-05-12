#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "blkmap.h"

FILE *devicefp;

int write_cnt;
int erase_cnt;

/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);
void dd_read(int ppn, char *pagebuf);
int main(int argc, char *argv[])
{
	char *blockbuf;
	char sectorbuf[SECTOR_SIZE];
	int lsn, i;

	devicefp = fopen("flashmemory", "w+b");
	if(devicefp == NULL)
	{
		printf("file open error\n");
		exit(1);
	}

	blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);

	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);
	}

	free(blockbuf);

	ftl_open();    // ftl_read(), ftl_write() 호출하기 전에 이 함수를 반드시 호출해야 함

	ftl_write(0,"A0");
	ftl_write(1,"A1");
	ftl_write(2,"A2");
	ftl_write(3,"A3");
	ftl_write(4,"A4");
	ftl_write(5,"B0");
	ftl_write(6,"B1");
	ftl_write(7,"B2");
	ftl_write(8,"B3");
	ftl_write(9,"B4");

	ftl_write(10,"C0");
	ftl_write(11,"C1");
	ftl_write(12,"C2");
	ftl_write(13,"C3");
	ftl_write(14,"C4");

	ftl_write(15,"D0");
	ftl_write(16,"D1");
	ftl_write(17,"D2");
	ftl_write(18,"D3");
	ftl_write(19,"D4");

	ftl_write(20,"E0");
	ftl_write(21,"E1");
	ftl_write(22,"E2");
	ftl_write(23,"E3");
	ftl_write(24,"E4");

	ftl_write(1,"a1");
	ftl_write(2,"a2");
	ftl_write(5,"b0");
	ftl_write(5,"b1");
	ftl_write(1,"newa1");
	ftl_write(2,"newa2");
	ftl_write(3,"newa3");
	ftl_write(4,"newa4");
	ftl_write(5,"newb2");
	

	int j;
	char pagebuf[PAGE_SIZE];
	char buf[4];

	printf("\n>>>>>>>>>>>>>> WRITE <<<<<<<<<<<<<<\n\n");
	for(i=0;i<6;i+=2){
		for(j=0;j<7;j++){
			memset(pagebuf,'\0',sizeof(pagebuf));
			memset(buf,'\0',sizeof(buf));
			dd_read(i*7+j,pagebuf);
			if(pagebuf[0]==-1 && pagebuf[1]==-1 && pagebuf[2]==-1){
				printf("[ NULL ]");
			}else{
				strncpy(buf,pagebuf,4);
				buf[3]='\0';
				printf("[  %s  ]",buf);
			}
			printf("                   ");
		
			memset(pagebuf,'\0',sizeof(pagebuf));
			memset(buf,'\0',sizeof(buf));
			dd_read((i+1)*7+j,pagebuf);
			if(pagebuf[0]==-1 && pagebuf[1]==-1 && pagebuf[2]==-1){
				printf("[ NULL ]\n");
			}else{
				strncpy(buf,pagebuf,4);
				buf[3]='\0';
				printf("[  %s  ]\n",buf);
			}
		}
		printf("\n");
	}
	printf("===================================\n");
	
	ftl_read(0,sectorbuf);
	ftl_read(1,sectorbuf);
	ftl_read(2,sectorbuf);
	ftl_read(3,sectorbuf);
	ftl_read(4,sectorbuf);
	ftl_read(5,sectorbuf);

	fclose(devicefp);

	return i;
}

