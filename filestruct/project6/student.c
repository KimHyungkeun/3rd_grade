#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "student.h"

//
// �л� ���ڵ� ���Ϸκ���  ���ڵ� ��ȣ�� �ش��ϴ� ���ڵ带 �о� ���ڵ� ���ۿ� �����Ѵ�.
//
/*void readStudentRec(FILE *fp, char *recordbuf, int rn){

}

//
// Hash file�κ��� rn�� ���ڵ� ��ȣ�� �ش��ϴ� ���ڵ带 �о� ���ڵ� ���ۿ� �����Ѵ�.
//
void readHashRec(FILE *fp, char *recordbuf, int rn){

}

//
// Hash file�� rn�� ���ڵ� ��ȣ�� �ش��ϴ� ��ġ�� ���ڵ� ������ ���ڵ带 �����Ѵ�.
//
void writeHashRec(FILE *fp, const char *recordbuf, int rn){

}*/

//
// n�� ũ�⸦ ���� hash file���� �־��� �й� Ű���� hashing�Ͽ� �ּҰ�(���ڵ� ��ȣ)�� �����Ѵ�.
//
int hashFunction(const char *sid, int n){

	FILE *hsh_fp;
	const char *ptr;
	char letters[3];
	int len;
	int multiple_result;
	int home_address;
	int is_full = 0;

	hsh_fp = fopen(HASH_FILE_NAME, "r");
	len = strlen(sid);
	ptr = sid + len - 2;
	strcpy(letters,ptr);

	multiple_result = letters[0] * letters[1];
	home_address = multiple_result % n; 

	return home_address;
}

//
// n�� ũ�⸦ ���� hash file�� �����Ѵ�.
// Hash file�� fixed length record ������� ����Ǹ�, ���ڵ��� ũ��� 14 ����Ʈ�̴�.
// (student.h ����)
//
void makeHashfile(int n){

	FILE* hsh_fp;
	FILE* dat_fp;
	struct stat statbuf;
	int hsh_size;  
	int filesize = 4;
	int home_address;
	int actual_address;
	int is_full = 0;
	int dat_offset;
	char dummy = '\0';
	char recordbuf[HASH_RECORD_SIZE];
	char sid[11];

	
	dat_fp = fopen(RECORD_FILE_NAME, "r");
	hsh_fp = fopen(HASH_FILE_NAME, "w+");
	hsh_size = filesize + (HASH_RECORD_SIZE * n);
	
	//printf("Real size : %d\n", hsh_size_except_filesize);

	stat(RECORD_FILE_NAME, &statbuf);
	//printf("statbuf size : %ld\n", statbuf.st_size);

	for(int i = 0 ; i < hsh_size ; i++) {

		fwrite(&dummy, sizeof(char) , 1, hsh_fp);
	}

	fseek(hsh_fp, 0 , SEEK_SET);
	fwrite(&n , sizeof(int) ,1 ,hsh_fp);
	

	for (int offset = 0 ; offset < statbuf.st_size ; offset += 120) {
		
		fseek(dat_fp, offset , SEEK_SET);
		fread(sid, sizeof(sid)-1, 1 , dat_fp);
		dat_offset = offset/120;
		
		home_address = hashFunction(sid, n);
		actual_address = home_address;
		//printf("home address : %d\n", home_address);
		
		while(is_full != n) {

			fseek(hsh_fp, filesize + (actual_address * HASH_RECORD_SIZE) , SEEK_SET);
			fread(recordbuf, sizeof(recordbuf), 1, hsh_fp);
			fseek(hsh_fp, filesize + (actual_address * HASH_RECORD_SIZE) , SEEK_SET);

			if(strlen(recordbuf) == 0) {
				fwrite(sid, strlen(sid) ,1 ,hsh_fp);
				fseek(hsh_fp, sizeof(sid) - strlen(sid) - 1, SEEK_CUR);
				fwrite(&dat_offset, sizeof(int) ,1 , hsh_fp);
				break;
			}

			else {
				is_full++;
				actual_address++;

				if(actual_address >= n)
					actual_address = 0;
				
			}

			memset(recordbuf , 0 , sizeof(recordbuf));

		}

		memset(sid , 0 , sizeof(sid));
		
	}

	fclose(hsh_fp);
	fclose(dat_fp);
	
}

//
// �־��� �й� Ű���� hash file���� �˻��Ѵ�.
// �� ����� �־��� �й� Ű���� �����ϴ� hash file������ �ּ�(���ڵ� ��ȣ)�� search length�̴�.
// �˻��� hash file������ �ּҴ� rn�� �����ϸ�, �̶� hash file�� �־��� �й� Ű����
// �������� ������ rn�� -1�� �����Ѵ�. (search()�� delete()������ Ȱ���� �� ����)
// search length�� �Լ��� ���ϰ��̸�, �˻� ����� ������� search length�� �׻� ���Ǿ�� �Ѵ�.
//
int search(const char *sid, int *rn){

	FILE *hsh_fp;
	struct stat statbuf;
	int filesize = 4;
	int search_length = 0;
	int home_address;
	int actual_address;
	int count = 0;
	int n;

	char recordbuf[HASH_RECORD_SIZE];
	

	hsh_fp = fopen(HASH_FILE_NAME, "r");
	stat(HASH_FILE_NAME, &statbuf);
	n = ( statbuf.st_size - filesize )/ HASH_RECORD_SIZE;

	home_address = hashFunction(sid, n);
	
	
	for (actual_address = 0 ; actual_address < n ; actual_address++) {
			fseek(hsh_fp , filesize + (actual_address * HASH_RECORD_SIZE), SEEK_SET);
			fread(recordbuf, SID_FIELD_SIZE , 1, hsh_fp);
			if (strcmp(sid, recordbuf) == 0) {
				break;
			}
	}

	if (actual_address == n) {
		*rn = -1;
		fclose(hsh_fp);
		return search_length;
	}
		


	if (home_address > actual_address) {
			search_length = n - home_address;
			search_length += actual_address;
			search_length++;
			
	}

	else if (home_address == actual_address){
			search_length++;
	}
		
	else {
			search_length = actual_address - home_address;
			search_length++;
	}
	

	fclose(hsh_fp);
	return search_length;
}

//
// Hash file���� �־��� �й� Ű���� ��ġ�ϴ� ���ڵ带 ã�� �� �ش� ���ڵ带 ���� ó���Ѵ�.
// �̶� �л� ���ڵ� ���Ͽ��� ���ڵ� ������ �ʿ����� �ʴ�.
//
void delete(const char *sid){

	FILE *hsh_fp;
	struct stat statbuf;
	int i = 0;
	int n = 0;
	int filesize = 4;
	char recordbuf[HASH_RECORD_SIZE];
	char delete_mark = '*';

	
	hsh_fp = fopen(HASH_FILE_NAME, "r+");
	stat(HASH_FILE_NAME, &statbuf);
	n = (statbuf.st_size - filesize) / HASH_RECORD_SIZE;

	for (i = 0 ; i < n ; i++) {
		fseek(hsh_fp , filesize + (i * HASH_RECORD_SIZE) , SEEK_SET);
		fread(recordbuf, SID_FIELD_SIZE, 1 , hsh_fp);
		
		if (strcmp(recordbuf, sid) == 0) {
			fseek(hsh_fp , filesize + (i * HASH_RECORD_SIZE) , SEEK_SET);
			fwrite(&delete_mark, sizeof(char) , 1, hsh_fp);
			break;
		}
	}


	fclose(hsh_fp);

}

//
// rn�� hash file������ ���ڵ� ��ȣ��, sl�� search length�� �ǹ��Ѵ�.
//
void printSearchResult(int rn, int sl)
{
	printf("%d %d\n", rn, sl);
}


// student.dat ?????? ?????.

int main(int argc, char* argv[])
{
	FILE *dat_fp;
	FILE *hsh_fp;
	struct stat statbuf;
    int i, n, dat_record;
	int filesize = 4;

    char packbuf[STUDENT_RECORD_SIZE] = {0, };
    char *ptr = packbuf;
	char input_sid[11];
	char sid[11];
	int rn;
	int search_length;

	

	if (argc < 2) {
		fprintf(stderr, "Usage : %s [OPTION]\n", argv[0]);
		exit(1);
	}

	if(strcmp(argv[1], "-c") == 0) {

		n = atoi(argv[2]);
		makeHashfile(n);

	}
    
	else if(strcmp(argv[1], "-s") == 0) {
		strcpy(input_sid, argv[2]);
		
		hsh_fp = fopen(HASH_FILE_NAME, "r");

		stat(HASH_FILE_NAME, &statbuf);
		n = (statbuf.st_size - filesize) / HASH_RECORD_SIZE;

		for(i = 0 ; i < n ; i++) {
			fseek(hsh_fp , filesize + (i * HASH_RECORD_SIZE) , SEEK_SET);
			fread(sid, SID_FIELD_SIZE, 1 , hsh_fp);
			
		
			if (strcmp(sid, input_sid) == 0) {
				search_length = search(sid, &i);
				printSearchResult(i, search_length);
				break;
			}

		}

		if (i == n) {
			rn = -1;
		}

		fclose(hsh_fp);
		
	}

	else if(strcmp(argv[1], "-d") == 0) {
		strcpy(sid, argv[2]);
		delete(sid);
	}


	   
    return 0;
}
