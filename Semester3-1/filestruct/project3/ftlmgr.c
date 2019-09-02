// 주의사항
// 1. blkmap.h에 정의되어 있는 상수 변수를 우선적으로 사용해야 함
// 2. blkmap.h에 정의되어 있지 않을 경우 본인이 만들어서 사용하면 됨

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"
// 필요한 경우 헤더 파일을 추가하시오.

int dd_read(int, char*);
int dd_write(int, char*);
int dd_erase(int);

extern FILE *devicefp;


int sparebuf[PAGES_PER_BLOCK * BLOCKS_PER_DEVICE]; //스페어를 관리하는 배열 
char freeblock = DATABLKS_PER_DEVICE; //freeblock으로 둘 위치를 지정한다


const int lsn_count = PAGES_PER_BLOCK * DATABLKS_PER_DEVICE; //lsn 갯수 
const int psn_count = PAGES_PER_BLOCK * DATABLKS_PER_DEVICE; //psn 갯수 
const int lbn_count = DATABLKS_PER_DEVICE; //lbn의 갯수
const int pbn_count = BLOCKS_PER_DEVICE; //pbn의 갯수

int lbn_table[DATABLKS_PER_DEVICE]; //lbn 테이블, 초기에 pbn 또한 똑같이 맵핑

int lbn; 
int pbn;
int ppn;
int offset;



//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다. 따라서, 첫 번째 ftl_write() 또는 ftl_read()가 호출되기 전에
// main()함수에서 반드시 먼저 호출이 되어야 한다.
//


// address mapping table 생성 및 초기화 등을 진행
// mapping table에서 lbn과 pbn의 수는 blkmap.h에 정의되어 있는 DATABLKS_PER_DEVICE
// 수와 같아야 하겠지요? 나머지 free block 하나는 overwrite 시에 사용하면 됩니다.
// pbn 초기화의 경우, 첫 번째 write가 발생하기 전을 가정하므로 예를 들면, -1로 설정을
// 하고, 그 이후 필요할 때마다 block을 하나씩 할당을 해 주면 됩니다. 어떤 순서대로 할당하는지는
// 각자 알아서 판단하면 되는데, free block들을 어떻게 유지 관리할 지는 스스로 생각해 보기
// 바랍니다.
void ftl_open()
{
	

	for(int pbn = 0 ; pbn < lbn_count ; pbn++){
		lbn_table[pbn] = pbn; //number of lbn
		
	}

	
	for(int ppn = 0 ; ppn < (PAGES_PER_BLOCK * BLOCKS_PER_DEVICE) ; ppn++) {
		sparebuf[ppn] = -1; //초기 spare들은 -1로 변환한다.
	}


	return;
}

//
// file system이 ftl_write()를 호출하면 FTL은 flash memory에서 주어진 lsn과 관련있는
// 최신의 데이터(512B)를 읽어서 sectorbuf가 가리키는 곳에 저장한다.
// 이 함수를 호출하기 전에 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 한다.
// 즉, 이 함수에서 메모리를 할당받으면 안된다.
//
void ftl_read(int lsn, char *sectorbuf)
{
	char pagebuf[PAGE_SIZE]; //한 페이지의 내용을 담을것이다
	lbn = lsn / PAGES_PER_BLOCK; 
	pbn = lbn_table[lbn]; //해당 lbn에 대응되는 pbn 값
	offset = (lsn % PAGES_PER_BLOCK); //offset
	ppn = offset + (PAGES_PER_BLOCK * pbn) ; //pure offset
	
	dd_read(ppn, pagebuf); //flash memory로부터 데이터를 읽어들인다
	strncpy(sectorbuf, pagebuf, SECTOR_SIZE); //pagebuf로부터 sector크기만큼 읽어들인다.

	printf("%s, spare : %d, ppn : %d\n",sectorbuf,pagebuf[SECTOR_SIZE],ppn);	
	return;
}


//
// file system이 ftl_write()를 호출하면 FTL은 flash memory에 sectorbuf가 가리키는 512B
// 데이터를 저장한다. 당연히 flash memory의 어떤 주소에 저장할 것인지는 block mapping 기법을
// 따라야한다.
//

void ftl_write(int lsn, char *sectorbuf)
{
	char pagebuf[PAGE_SIZE];//페이지의 내용을 담을배열
	lbn = lsn / PAGES_PER_BLOCK; 
	pbn = lbn_table[lbn]; //lbn table을 읽어서, pbn과 대응되는 값을 찾음
	offset = (lsn % PAGES_PER_BLOCK); //offset
	ppn = offset + (PAGES_PER_BLOCK * pbn) ; //ppn

	
	
	dd_read(ppn, pagebuf); //flashmemory로부터 값을 읽어들인다.


			if(pagebuf[SECTOR_SIZE] == -1) { //유저가 데이터를 새로 넣는 경우
				
				strncpy(pagebuf,sectorbuf, SECTOR_SIZE);		
				pagebuf[SECTOR_SIZE] = 0; //input spare area
				sparebuf[ppn] = 0; //데이터가 들어갔으므로 spare는 0이다
				dd_write(ppn, pagebuf);
				memset(pagebuf,0,PAGE_SIZE); //해당 버퍼를 다시 깨끗이 비운다
			}

			else if(pagebuf[SECTOR_SIZE] == 0){ //if user overwrite data	
			//printf("Check\n");
			
				for(int i=0; i < PAGES_PER_BLOCK; i++) { //유저가 기존 데이터를 업데이트 하는 경우
					if(i == offset){ 
						strncpy(pagebuf,sectorbuf, SECTOR_SIZE); 
						pagebuf[SECTOR_SIZE] = 0; 
						sparebuf[PAGES_PER_BLOCK * freeblock + i] = 0; //freeblock쪽의 ppn의 spare을 0으로 
						dd_write(PAGES_PER_BLOCK * freeblock + i, pagebuf);//해당하는 freeblock에 데이터넣음
						sparebuf[pbn * PAGES_PER_BLOCK + i] = -1; //기존 블록의 spare는 -1로 초기화
						
					}
					
					else {					
						dd_read(pbn * PAGES_PER_BLOCK + i, pagebuf); //업데이트하려는 offset을 제외한 나머지를 읽어들인다.
						dd_write(PAGES_PER_BLOCK * freeblock + i, pagebuf); //대응하는 offset에 맞게 데이터를 넣는다
						sparebuf[PAGES_PER_BLOCK * freeblock + i] = sparebuf[pbn * PAGES_PER_BLOCK + i]; //기존 위치의 spare를 새 freeblock쪽의 spare에 그대로 넣는다.(복사한다)
						sparebuf[pbn * PAGES_PER_BLOCK + i] = -1; //이전 블록의 spare는 -1로 초기화
						
					}

				}

				dd_erase(lbn_table[lbn]); //이전의 block의 데이터들을 전부 clear 시킨다
				lbn_table[lbn] = freeblock; 
				freeblock = pbn; //이전의 block 위치가 freeblock 위치로  변경된다.
				
			}
	
	
	return;
}
