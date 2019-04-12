#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "blkmap.h"

FILE *devicefp;

/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);

//
// 이 함수는 file system의 역할을 수행한다고 생각하면 되고,
// file system이 flash memory로부터 512B씩 데이터를 저장하거나 데이터를 읽어 오기 위해서는
// 본인이 구현한 FTL의 ftl_write()와 ftl_read()를 호출하면 됨
//
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
	   
    //
    // flash memory의 모든 바이트를 '0xff'로 초기화한다.
    // 
    blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);

	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);
	}

	free(blockbuf);

	ftl_open();    // ftl_read(), ftl_write() 호출하기 전에 이 함수를 반드시 호출해야 함

	/*lsn = 0;
	strcpy(sectorbuf, "First Sentence");
	ftl_write(lsn,sectorbuf);
	//ftl_read(lsn,sectorbuf);

	lsn = 1;
	strcpy(sectorbuf, "Second Sentence");
	ftl_write(lsn,sectorbuf);
	//ftl_read(lsn,sectorbuf);

	lsn = 2;
	strcpy(sectorbuf, "Third Sentence");
	ftl_write(lsn,sectorbuf);
	//ftl_read(lsn,sectorbuf);

	lsn = 2;
	strcpy(sectorbuf, "New Third Sentence");
	ftl_write(lsn,sectorbuf);
	//ftl_read(lsn,sectorbuf);

	lsn = 2;
	strcpy(sectorbuf, "SecondNew Third Sentence");
	ftl_write(lsn,sectorbuf);

	lsn = 26;
	strcpy(sectorbuf, "26 Sentence");
	ftl_write(lsn,sectorbuf);*/

	for (int lsn = 0; lsn < PAGES_PER_BLOCK * DATABLKS_PER_DEVICE ; lsn++){
		strcpy(sectorbuf,"Sentence");
		ftl_write(lsn,sectorbuf);
	}

	strcpy(sectorbuf,"Yeah!!!!");
	ftl_write(6,sectorbuf);

	strcpy(sectorbuf,"OHNOYeah!!!!");
	ftl_write(6,sectorbuf);

	ftl_read(0,sectorbuf);
	ftl_read(1,sectorbuf);
	ftl_read(2,sectorbuf);
	ftl_read(26,sectorbuf);
	ftl_read(6,sectorbuf);
	//ftl_read(lsn,sectorbuf);

	
	//
	// ftl_write() 및 ftl_read() 테스트를 위한 코드를 자유자재로 만드세요
	//


	fclose(devicefp);

	return 0;
}
