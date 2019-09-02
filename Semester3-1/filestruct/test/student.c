#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "student.h"

void printRecord(const STUDENT *s);

//
// 학생 레코드 파일에 레코드를 저장하기 전에 구조체 STUDENT에 저장되어 있는 데이터를 레코드 형태로 만든다.
// 
void pack(char *recordbuf, const STUDENT *s){

	char* delimeter = "|";
	strcat(recordbuf, s -> id);
	strcat(recordbuf,delimeter);
	//printf("%s\n", recordbuf);
	strcat(recordbuf, s -> name);
	strcat(recordbuf,delimeter);
	//printf("%s\n", recordbuf);
	strcat(recordbuf, s -> dept);
	strcat(recordbuf,delimeter);
	//printf("%s\n", recordbuf);
	strcat(recordbuf, s -> year);
	strcat(recordbuf,delimeter);
	//printf("%s\n", recordbuf);
	strcat(recordbuf, s -> addr);
	strcat(recordbuf,delimeter);
	//printf("%s\n", recordbuf);
	strcat(recordbuf, s -> phone);
	strcat(recordbuf,delimeter);
	//printf("%s\n", recordbuf);
	strcat(recordbuf, s -> email);
	strcat(recordbuf,delimeter);
	//printf("%s\n", recordbuf);
}

// 
// 학생 레코드 파일로부터 레코드를 읽어 온 후 구조체 STUDENT로 변환한다.
//
void unpack(const char *recordbuf, STUDENT *s){

	char *ptr;
	char* delimeter = "|";
	char buf[MAX_RECORD_SIZE];
	strcpy(buf, recordbuf);

	ptr = strtok(buf, delimeter);
	strcpy(s -> id, ptr);
	ptr = strtok(NULL, delimeter);
	strcpy(s -> name, ptr);
	ptr = strtok(NULL, delimeter);
	strcpy(s -> dept, ptr);
	ptr = strtok(NULL, delimeter);
	strcpy(s -> year, ptr);
	ptr = strtok(NULL, delimeter);
	strcpy(s -> addr, ptr);
	ptr = strtok(NULL, delimeter);
	strcpy(s -> phone, ptr);
	ptr = strtok(NULL, delimeter);
	strcpy(s -> email, ptr);

	printRecord(s);

}

//
// 학생 레코드 파일로부터  레코드 번호에 해당하는 레코드를 읽어서 레코드 버퍼에 저장한다.
//
void readRecord(FILE *fp, char *recordbuf, int rn){

		FILE *idx_fp;
		struct stat dat_statbuf;
		struct stat idx_statbuf;

		short byte_offset;
		short next_byte_offset;
	
		stat(RECORD_FILE_NAME, &dat_statbuf);
		stat(INDEX_FILE_NAME,&idx_statbuf);
		idx_fp = fopen(INDEX_FILE_NAME, "r");
		fp = fopen(RECORD_FILE_NAME, "r");

		printf("found rn : %d\n", rn);
		fseek(idx_fp, rn*2 + sizeof(short), SEEK_SET);
		fread(&byte_offset, sizeof(short), 1, idx_fp);
		fseek(fp, sizeof(short), SEEK_SET);
		printf("found byte_offset : %d\n", byte_offset);

		if ((rn*2 + 2*sizeof(short)) == idx_statbuf.st_size) {
			fseek(fp, sizeof(short) + byte_offset, SEEK_SET);
			fread(recordbuf, dat_statbuf.st_size - byte_offset , 1, fp);
			//printf("%ld\n", dat_statbuf.st_size - byte_offset);
			recordbuf[strlen(recordbuf)] = 0;
			//printf("%c\n", recordbuf[strlen(recordbuf)- 2*sizeof(short)]);
		}

		else {
			fseek(fp, byte_offset, SEEK_CUR);
			fseek(idx_fp,rn*2 + 2*sizeof(short), SEEK_SET);
			fread(&next_byte_offset, sizeof(short) , 1 , idx_fp);
			fread(recordbuf, next_byte_offset - byte_offset, 1 , fp);
			//printf("%d\n", next_byte_offset - byte_offset);
			recordbuf[strlen(recordbuf)] = 0;
			//printf("%c\n", recordbuf[strlen(recordbuf)-sizeof(short)]);
		}

		fclose(fp);
		fclose(idx_fp);
}

//
//알고리즘: 저장
//학생 레코드 파일에서 삭제 레코드의 존재 여부를 검사한 후 삭제 레코드가 존재하면 이 공간에
//새로운 레코드를 저장하며, 만약 삭제 레코드가 존재하지 않거나 조건에 부합하는 삭제 레코드가
//존재하지 않으면  파일의 맨마지막에 저장한다.
//
void add(FILE *fp, FILE *idx_fp, const STUDENT *s){

	struct stat dat_statbuf;
	struct stat idx_statbuf;
	char recordbuf[MAX_RECORD_SIZE] = "\0";
	short head;
	short record_count;
	short byte_offset;
	short len;
	
	stat(RECORD_FILE_NAME, &dat_statbuf);
	stat(INDEX_FILE_NAME, &idx_statbuf);

	fp = fopen(RECORD_FILE_NAME, "r");
	idx_fp = fopen(INDEX_FILE_NAME, "r");

	fread(&head, sizeof(short), 1, fp);
	fread(&record_count, sizeof(short), 1, idx_fp);
	

	pack(recordbuf,s);
	fclose(fp);
	fclose(idx_fp);

	fp = fopen(RECORD_FILE_NAME, "r+");
	idx_fp = fopen(INDEX_FILE_NAME, "r+");

	if (head == -1) {
		record_count++;
		//printf("head : %d\n", head);
		printf("record_count : %d\n", record_count);

		byte_offset = dat_statbuf.st_size - sizeof(short);
		fwrite(&record_count, sizeof(short), 1, idx_fp);
		fseek(idx_fp , 0 , SEEK_END);
		fwrite(&byte_offset, sizeof(short), 1, idx_fp);

		fwrite(&head, sizeof(short), 1, fp);
		fseek(fp, 0 , SEEK_END);
		fwrite(recordbuf, strlen(recordbuf), 1, fp);
	}

	else {
		fseek(fp, sizeof(short) + head + 3, SEEK_SET);
		fread(&len, sizeof(short), 1 , fp);
		//printf("len : %d\n", len);
		//printf("strlen : %ld\n", strlen(recordbuf));
		//printf("strlen : %s\n", recordbuf);

		if (strlen(recordbuf) > len) {
			record_count++;
			//printf("head : %d\n", head);
			//printf("record_count : %d\n", record_count);

			byte_offset = dat_statbuf.st_size - sizeof(short);
			fwrite(&record_count, sizeof(short), 1, idx_fp);
			fseek(idx_fp , 0 , SEEK_END);
			fwrite(&byte_offset, sizeof(short), 1, idx_fp);

			fwrite(&head, sizeof(short), 1, fp);
			fseek(fp, 0 , SEEK_END);
			fwrite(recordbuf, strlen(recordbuf), 1, fp);
		}

		else {
			fseek(fp , 0 , SEEK_SET);
			fread(&head, sizeof(short) ,1 ,fp);
			byte_offset = head;
			fseek(fp , sizeof(short) + byte_offset + 1, SEEK_SET);
			fread(&head, sizeof(short) , 1 , fp);
			fseek(fp, 0 , SEEK_SET);
			fwrite(&head, sizeof(short), 1, fp);
			fseek(fp , sizeof(short) + byte_offset, SEEK_SET);
			fwrite(recordbuf, strlen(recordbuf), 1, fp);
		}
	}

	fclose(fp);
	fclose(idx_fp);
	
}

//
//알고리즘: 검색
//학생 레코드 파일에서 sequential search 기법으로 주어진 학번 키값고 일치하는 레코드를 찾는다.
//출력은 만드시 주어진 print 함수를 사용한다.
//검색 레코드가 존재하면 레코드 번호 rn을, 그렇지 않으면 -1을 리턴한다.
//
int search(FILE *fp,const char *keyval){

	FILE *idx_fp;
	struct stat dat_statbuf;
	stat(RECORD_FILE_NAME, &dat_statbuf);
	struct stat idx_statbuf;
	stat(INDEX_FILE_NAME, &idx_statbuf);
	char recordbuf[dat_statbuf.st_size];
	char testbuf[dat_statbuf.st_size];
	char onlykey[11] = "\0";
	char* ptr;
	
	short byte_offset;
	short next_byte_offset;
	short offset_check;
	int seek;
	int i;
	int word_count;
	int not_found = 0;

	short count_record;
	short len;

	fp = fopen(RECORD_FILE_NAME, "r");
	idx_fp = fopen(INDEX_FILE_NAME, "r");

	fseek(fp , sizeof(short), SEEK_SET);
	fread(recordbuf, dat_statbuf.st_size, 1 , fp);
	fread(&count_record, sizeof(short), 1 ,idx_fp);
	
	for(i = 1 ; i <= count_record ; i++) {
		
		fseek(idx_fp , i*2 , SEEK_SET);
		fread(&byte_offset, sizeof(short) , 1, idx_fp);

		if( i*2 + sizeof(short) == idx_statbuf.st_size )
			next_byte_offset = dat_statbuf.st_size - sizeof(short);

		else {
			fseek(idx_fp, i*2 + sizeof(short) , SEEK_SET);
			fread(&next_byte_offset, sizeof(short), 1, idx_fp);
			fseek(idx_fp , i*2 , SEEK_SET);
		}

		//printf("byte_offset move : %d\n", byte_offset);
		len = next_byte_offset - byte_offset;
		snprintf(testbuf, len,"%s",recordbuf + byte_offset);
		//printf("len : %d\n", len);
		//printf("testbuf : %s\n", testbuf);
		
		for(word_count = 0 ; word_count < strlen(testbuf) ; word_count++) {
			if(testbuf[word_count] == '|') 
				break;
		}

		
		snprintf(onlykey, word_count+1 , "%s" , testbuf);
		//printf("onlykey : %s\n", onlykey);

		if(strcmp(onlykey, keyval) != 0) {
			not_found++;
			continue;
		}

		else {
			//printf("ptr : %s\n", ptr);
			//printf("not_found : %d\n", not_found);
			break;
		}
	}

	if (not_found == count_record) {
		fprintf(stderr,"Could not found record\n");
		return -1;
	}

	for (seek = sizeof(short) ; seek <= idx_statbuf.st_size - sizeof(short) ; seek+=2) {
		fseek(idx_fp, seek, SEEK_SET);
		fread(&offset_check, sizeof(short), 1, idx_fp);
		//printf("%d\n", seek);
		//printf("%d %d\n", offset_check, byte_offset);
		if(offset_check == byte_offset) {
			
			seek /= 2;
			break;
		}
		fseek(idx_fp, 0, SEEK_SET);
	}

	fseek(idx_fp, 0, SEEK_SET);
	fclose(fp);
	fclose(idx_fp);

	
	return --seek;
}

//
//알고리즘: 삭제
//학생 파일에서 주어진 학번 키값과 일치하는 레코드를 찾은 후 해당 레코드를 삭제 처리한다.
//
void delete(FILE *fp, const char *keyval){

	

	FILE *idx_fp;
	struct stat dat_statbuf;
	stat(RECORD_FILE_NAME, &dat_statbuf);
	struct stat idx_statbuf;
	stat(INDEX_FILE_NAME, &idx_statbuf);
	char recordbuf[dat_statbuf.st_size];
	char* ptr;
	char testbuf[dat_statbuf.st_size];
	char onlykey[11] = "\0";
	
	char delete_char = '*';
	short head;
	short byte_offset;
	short next_byte_offset;
	short check_offset;
	short len;
	short rn;
	int word_count;
	int i;
	int not_found = 0;

	short count_record;
	

	fp = fopen(RECORD_FILE_NAME, "r+");
	idx_fp = fopen(INDEX_FILE_NAME, "r+");

	fread(&head, sizeof(short), 1 ,fp);

	fseek(fp , sizeof(short), SEEK_SET);

	fread(recordbuf, dat_statbuf.st_size, 1 , fp);
	fread(&count_record, sizeof(short), 1 ,idx_fp);
	
	for(i = 1 ; i <= count_record ; i++) {
		
		fseek(idx_fp , i*2 , SEEK_SET);
		fread(&byte_offset, sizeof(short) , 1, idx_fp);

		if( i*2 + sizeof(short) == idx_statbuf.st_size )
			next_byte_offset = dat_statbuf.st_size - sizeof(short);

		else {
			fseek(idx_fp, i*2 + sizeof(short) , SEEK_SET);
			fread(&next_byte_offset, sizeof(short), 1, idx_fp);
			fseek(idx_fp , i*2 , SEEK_SET);
		}

		len = next_byte_offset - byte_offset;
		snprintf(testbuf, len, "%s",recordbuf + byte_offset);
	

		for (word_count = 0; word_count < strlen(testbuf) ; word_count++) {
			if(testbuf[word_count] == '|')
				break;
		}

		snprintf(onlykey, word_count+1 , "%s" , testbuf);
		
		if(strcmp(onlykey, keyval) != 0) {
			not_found++;
			continue;
		}

		else {
			break;
		}
	}

	if (not_found == count_record) {
		fprintf(stderr,"Cannot find key_value(Delete Failed)\n");
		return;
	}

	
	fseek(fp, byte_offset, SEEK_CUR);
	//printf("Cutted : %s\n", ptr);
	//printf("byte_offset : %d\n", byte_offset);

	for (rn = sizeof(short) ; rn <= idx_statbuf.st_size - sizeof(short) ; rn+=2) {
		fseek(idx_fp, rn, SEEK_SET);
		//printf("%d\n", seek);
		//printf("%d %d\n", offset_check, byte_offset);
		fread(&check_offset, sizeof(short), 1, idx_fp);
		if(check_offset == byte_offset) {
			break;
		}
		fseek(idx_fp, 0, SEEK_SET);
	}

	fseek(idx_fp , rn + sizeof(short) , SEEK_SET);
	fread(&next_byte_offset, sizeof(short) , 1 ,idx_fp);
	

	if((rn + sizeof(short)) == idx_statbuf.st_size) {
		next_byte_offset = dat_statbuf.st_size - sizeof(short);
		len = next_byte_offset - byte_offset;
	}

	else {
		len = next_byte_offset - byte_offset;
	}

	//printf("next_byte_offset : %d\n", next_byte_offset);
	//printf("len : %d\n", len);

	fseek(fp, sizeof(short) + byte_offset, SEEK_SET);
	fwrite(&delete_char, sizeof(char), 1, fp);
	fwrite(&head, sizeof(short), 1, fp);
	fwrite(&len, sizeof(short), 1 , fp);

	head = byte_offset;
	fseek(fp , 0 , SEEK_SET);
	fwrite(&head, sizeof(short), 1, fp);

	fclose(fp);
	fclose(idx_fp);

}

void printRecord(const STUDENT *s)
{
		printf("%s|%s|%s|%s|%s|%s|%s\n", s -> id, s -> name, s -> dept, s -> year, s -> addr, s -> phone, s -> email);
}


int main(int argc, char *argv[])
{
	
	FILE *fp;  // 학생 레코드 파일의 파일 포인터
	FILE *idx_fp; // 레코드 파일을 다루는 인덱스 파일
	STUDENT s; //학생 구조체

	short record_count = 0; //레코드들의 수
	short head = -1; //삭제 레코드를 다루는 헤더
	

	int rn; //레코드 번호
	char recordbuf[MAX_RECORD_SIZE]; //레코드를 담을 버퍼


	
	if (argc < 3) {
		fprintf(stderr, "Usage : %s -a \"field_value1\" \"field_value2\" ... \"field_value7\" \n", argv[0]);
		fprintf(stderr, "Usage : %s -s \"key_value\" \n", argv[0]);
		fprintf(stderr, "Usage : %s -d \"key_value\" \n", argv[0]);
		exit(1);
	}

	if ( access(RECORD_FILE_NAME, F_OK) < 0 || access(INDEX_FILE_NAME, F_OK) < 0 ) {
			fp = fopen(RECORD_FILE_NAME, "w+");
			idx_fp = fopen(INDEX_FILE_NAME, "w+");
			fwrite(&head, sizeof(short), 1 ,fp);
			fwrite(&record_count, sizeof(short), 1 ,idx_fp);
			fclose(fp);
			fclose(idx_fp);
	}

	
	if (strcmp(argv[1],"-a") == 0) { //add 옵션
		if (argc != 9) {
			fprintf(stderr, "You must input 7 fields!\n");
			exit(1);
		}
		
		strncpy(s.id, argv[2],sizeof(s.id) );
		strncpy(s.name, argv[3], sizeof(s.name) );
		strncpy(s.dept, argv[4], sizeof(s.dept) );
		strncpy(s.year, argv[5], sizeof(s.year) );
		strncpy(s.addr, argv[6], sizeof(s.addr) );
		strncpy(s.phone, argv[7], sizeof(s.phone) );
		strncpy(s.email, argv[8], sizeof(s.email) );
		add(fp, idx_fp ,&s);
			
	}

	else if (strcmp(argv[1],"-s") == 0) { //search 옵션
		if (argc > 3) {
			fprintf(stderr, "Only input 1 key_value to search!\n");
			exit(1);
		}

		rn = search(fp, argv[2]);
		
		if(rn == -1) {
			printf("Key value does no exist.\n");
			exit(1);
		}

		readRecord(fp, recordbuf, rn);
		unpack(recordbuf,&s);
		
	}

	else if (strcmp(argv[1],"-d") == 0) { //delete 옵션
		if (argc > 3) {
			fprintf(stderr, "Only input 1 key_value to delete!\n");
			exit(1);
		}

		delete(fp, argv[2]);
		
	}

	else {
		printf("Wrong Option\n");
		exit(1);
	}

	short dat;
	fp = fopen(RECORD_FILE_NAME, "r");
	fread(&dat, sizeof(short), 1 ,fp);
	printf("head : %d\n", dat);
	fclose(fp);
	
	
	return 1;
}









