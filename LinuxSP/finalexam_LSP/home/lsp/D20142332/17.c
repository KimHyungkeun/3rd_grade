#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>

typedef struct backupList {

	struct backupList* next;
	char filename[255];
}ssu_backupList;

int addFun(char *filename);
int RemoveFun(char *filename);
ssu_backupList *listHEAD;

int main(int argc, char *argv[]) {

	listHEAD = (ssu_backupList *)malloc(sizeof(ssu_backupList));

	addFun("a.txt");
	addFun("b.txt");
	addFun("c.txt");

	RemoveFun("b.txt");
	RemoveFun("c.txt");
	RemoveFun("a.txt");

	free(listHEAD);

	exit(0);
}

int addFun (char *filename) {

	ssu_backupList *listadd = (ssu_backupList *)malloc(sizeof(ssu_backupList));

	strcpy(listadd -> filename, filename);
	listadd->next = NULL;

	ssu_backupList *seek = listHEAD;
	
	while(seek -> next != NULL) {
	
		seek = seek -> next;
	}
	seek->next = listadd;

	return 0;
}

int RemoveFun(char *filename) {

	ssu_backupList *seek = listHEAD -> next;
	ssu_backupList *prev = listHEAD;


	printf("del name =%s\n", filename);

	return 1;

}
