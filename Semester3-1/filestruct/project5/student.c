#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "student.h"

void printRecord(const STUDENT *s);

//
// �л� ���ڵ� ���Ͽ� ���ڵ带 �����ϱ� ���� ����ü STUDENT�� ����Ǿ� �ִ� �����͸� ���ڵ� ���·� �����.
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
// �л� ���ڵ� ���Ϸκ��� ���ڵ带 �о� �� �� ����ü STUDENT�� ��ȯ�Ѵ�.
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
// �л� ���ڵ� ���Ϸκ���  ���ڵ� ��ȣ�� �ش��ϴ� ���ڵ带 �о ���ڵ� ���ۿ� �����Ѵ�.
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

		//printf("found rn : %d\n", rn);
		fseek(idx_fp, rn*2 + sizeof(short), SEEK_SET);
		fread(&byte_offset, sizeof(short), 1, idx_fp);
		fseek(fp, sizeof(short), SEEK_SET);
		//printf("found byte_offset : %d\n", byte_offset);

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
//�˰�����: ����
//�л� ���ڵ� ���Ͽ��� ���� ���ڵ��� ���� ���θ� �˻��� �� ���� ���ڵ尡 �����ϸ� �� ������
//���ο� ���ڵ带 �����ϸ�, ���� ���� ���ڵ尡 �������� �ʰų� ���ǿ� �����ϴ� ���� ���ڵ尡
//�������� ������  ������ �Ǹ������� �����Ѵ�.
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
//�˰�����: �˻�
//�л� ���ڵ� ���Ͽ��� sequential search ������� �־��� �й� Ű���� ��ġ�ϴ� ���ڵ带 ã�´�.
//����� ����� �־��� print �Լ��� ����Ѵ�.
//�˻� ���ڵ尡 �����ϸ� ���ڵ� ��ȣ rn��, �׷��� ������ -1�� �����Ѵ�.
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
		//fprintf(stderr,"Could not found record\n");
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
//�˰�����: ����
//�л� ���Ͽ��� �־��� �й� Ű���� ��ġ�ϴ� ���ڵ带 ã�� �� �ش� ���ڵ带 ���� ó���Ѵ�.
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
		//fprintf(stderr,"Cannot find key_value(Delete Failed)\n");
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
	
	FILE *fp;  // �л� ���ڵ� ������ ���� ������
	FILE *idx_fp; // ���ڵ� ������ �ٷ�� �ε��� ����
	STUDENT s; //�л� ����ü

	short record_count = 0; //���ڵ���� ��
	short head = -1; //���� ���ڵ带 �ٷ�� ���
	

	int rn; //���ڵ� ��ȣ
	char recordbuf[MAX_RECORD_SIZE]; //���ڵ带 ���� ����


	
	if (argc < 3) {
		//fprintf(stderr, "Usage : %s -a \"field_value1\" \"field_value2\" ... \"field_value7\" \n", argv[0]);
		//fprintf(stderr, "Usage : %s -s \"key_value\" \n", argv[0]);
		//fprintf(stderr, "Usage : %s -d \"key_value\" \n", argv[0]);
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

	
	if (strcmp(argv[1],"-a") == 0) { //add �ɼ�
		if (argc != 9) {
			//fprintf(stderr, "You must input 7 fields!\n");
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

	else if (strcmp(argv[1],"-s") == 0) { //search �ɼ�
		if (argc > 3) {
			//fprintf(stderr, "Only input 1 key_value to search!\n");
			exit(1);
		}

		rn = search(fp, argv[2]);
		
		if(rn == -1) {
			//printf("Key value does no exist.\n");
			exit(1);
		}

		readRecord(fp, recordbuf, rn);
		unpack(recordbuf,&s);
		
	}

	else if (strcmp(argv[1],"-d") == 0) { //delete �ɼ�
		if (argc > 3) {
			//fprintf(stderr, "Only input 1 key_value to delete!\n");
			exit(1);
		}

		delete(fp, argv[2]);
		
	}

	else {
		//printf("Wrong Option\n");
		exit(1);
	}

	
	
	return 1;
}









