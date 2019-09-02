#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

typedef struct node {
    struct node *prev;
    int data;
    struct node *next;
} Node;

Node *head;
Node *curr;
int i = 0;
int num = 5;


int main(void) {

    head = (Node*)malloc(sizeof(Node));
    curr = head;
    curr -> prev = NULL;
    curr -> next = NULL;

    while ( i != 6 ) {
        curr -> next = (Node*)malloc(sizeof(Node));
        curr -> next -> prev = curr;
        curr -> next -> data = i;
        printf("%d ", curr -> next ->data);
        i++;
        curr -> next -> next = NULL;
        curr = curr -> next;
       
    }
    printf("\n");

    curr = head -> next;
    while (curr != NULL) {

        printf("%d ", curr -> data);
        curr = curr -> next;
    }
    printf("\n");

    curr = head -> next;
    while (curr != NULL) {

        if(curr -> data == num) {
            curr -> prev -> next = curr -> next;
            if(curr -> next == NULL)
                curr -> next = NULL;
            else 
                curr -> next -> prev = curr -> prev;

            free(curr);
            num--;
            printf("\n");
            curr = head -> next;
            
        }

        else {
        printf("%d ", curr -> data);
        curr = curr -> next;
        }
    }




    /*Node* node0 = (Node*)malloc(sizeof(Node));
    node0 -> next = NULL;

    Node* node1 = (Node*)malloc(sizeof(Node));
    node1 -> next = node0 -> next;
    node1 -> prev = node0;
    node1 -> data = 1; 
    node0 -> next = node1;
    

    Node* node2 = (Node*)malloc(sizeof(Node));
    node2 -> next = node1 -> next;
    node2 -> prev = node1;
    node2 -> data = 2; 
    node1 -> next = node2;

    Node* node3 = (Node*)malloc(sizeof(Node));
    node3 -> next = node2 -> next;
    node3 -> prev = node2;
    node3 -> data = 3; 
    node2 -> next = node3;

    Node* node4 = (Node*)malloc(sizeof(Node));
    node4 -> next = node3 -> next;
    node4 -> prev = node3;
    node4 -> data = 4; 
    node3 -> next = node4;

    Node* node5 = (Node*)malloc(sizeof(Node));
    node5 -> next = node4 -> next;
    node5 -> prev = node4;
    node5 -> data = 5; 
    node4 -> next = node5;

    Node* curr = node0 -> next;
    while (curr != NULL) {
        printf("%d ", curr -> data);
        curr = curr -> next;
    }

    printf("\n");

    curr = node0 -> next;
    while (curr != NULL) {
        
        if(curr -> data == 3) {
            curr -> prev -> next = curr -> next;
            if(curr -> next == NULL)
                curr -> next = NULL;
            else 
                curr -> next -> prev = curr -> prev;

            free(curr);
            curr = node0 -> next;
        }

        else
            curr = curr -> next;
    }

    curr = node0 -> next;
    while (curr != NULL) {

        printf("%d ", curr -> data);
        curr = curr -> next;
    }

    printf("\n");*/

    
    /*struct tm *tm_p;
    time_t now;
    char buf[255];
    char *pathname = "ssu_backup_directory";

    if (realpath(pathname, buf) == NULL) {
        fprintf(stderr, "path error\n");
        exit(1);
    }

    printf("full pathname :%s\n", buf);
    while(1) {
    time(&now);
    tm_p = localtime(&now);
    printf("Current time : [%d%02d%02d %02d%02d%02d]\n", tm_p -> tm_year - 100, tm_p -> tm_mon+1, tm_p -> tm_mday, tm_p -> tm_hour, tm_p -> tm_min, tm_p -> tm_sec);
    sleep(1);
    }
    exit(1);*/
    

}