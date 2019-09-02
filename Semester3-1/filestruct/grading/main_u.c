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
void dd_read(int, char *);

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

	char gbuf[BLOCKS_PER_DEVICE][BLOCK_SIZE] = {0};
	char lbuf[BLOCK_SIZE] = {0};

	ftl_write(0,"A+");
	//ftl_write(1,"A1");
	//ftl_write(2,"A2");
	
	ftl_write(3,"B+");
	//ftl_write(4,"B1");
	ftl_write(5,"B-");

	/////////////////////////////
	ftl_write(0,"A1");

	ftl_write(3,"B/");
	ftl_write(3,"B0");
	ftl_write(5,"B2");
	////////////////////////////////////////////////
	int ppn;
	char pagebuf[PAGE_SIZE];
	char dummy[PAGE_SIZE];

	memset(dummy, 0xff, sizeof(dummy));
	printf("\n>>>>>>>>>>>>>> Update  <<<<<<<<<<<<<<\n\n");
	for (int i = 0; i < BLOCKS_PER_DEVICE; i += 2)
	{
		for (int j = 0; j < PAGES_PER_BLOCK; j++)
		{
			/************ Block left *******************************/
			memset(pagebuf, 0, sizeof(pagebuf));
			memset(sectorbuf, 0, sizeof(sectorbuf));

			ppn = (i * PAGES_PER_BLOCK) + j;
			dd_read(ppn, pagebuf);

			if (memcmp(pagebuf, dummy, sizeof(pagebuf)) == 0)
			{
				if (j < NONBUF_PAGES_PER_BLOCK)
					printf("[ NULL ]");
				else
					printf("< NULL >"); 

			}
			else
			{
				memcpy(sectorbuf, pagebuf, sizeof(sectorbuf));

				if (j < NONBUF_PAGES_PER_BLOCK)
					printf("[  %s  ]", sectorbuf);
				else
					printf("<  %s  >", sectorbuf);

			}

			printf("                   ");
			/************ Block right *******************************/
			memset(pagebuf, 0, sizeof(pagebuf));
			memset(sectorbuf, 0, sizeof(sectorbuf));

			ppn = ((i + 1) * PAGES_PER_BLOCK) + j;
			dd_read(ppn, pagebuf);

			if (memcmp(pagebuf, dummy, sizeof(pagebuf)) == 0)
			{
				if (j < NONBUF_PAGES_PER_BLOCK)
					printf("[ NULL ]\n");
				else
					printf("< NULL >\n");

			}
			else
			{
				memcpy(sectorbuf, pagebuf, sizeof(sectorbuf));

				if (j < NONBUF_PAGES_PER_BLOCK)
					printf("[  %s  ]\n", sectorbuf);
				else
					printf("<  %s  >\n", sectorbuf);

			}
		}
		printf("\n");
	}
	printf("===================================\n");
	ftl_read(0, sectorbuf);
	printf("lsn[0]  : %s\n", sectorbuf); 

	ftl_read(3, sectorbuf);
	printf("lsn[3]  : %s\n", sectorbuf); 
	ftl_read(5, sectorbuf);
	printf("lsn[5]  : %s\n", sectorbuf);
	printf("===================================\n");

#ifdef GRADE
#endif

	fclose(devicefp);

	return 0;
}
