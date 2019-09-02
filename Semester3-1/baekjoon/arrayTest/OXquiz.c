#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define STR_LINE 10
#define X_SCORE 0

int main() {

    int num, i, j;
    int O_score = 1;
    int add_score = 2;
    int *total_score;
    char** str;
    char tmp[80];

    scanf("%d", &num);

    total_score = (int*)malloc(sizeof(int) * num);
    str = (char**)malloc(sizeof(char*) * num);

    for(i=0;i<num;i++)
        total_score[i] = 0;

    
        
    for(i=0;i<num;i++) {
        scanf("%s", tmp);
        str[i] = (char*)malloc(sizeof(char) * strlen(tmp));
        strcpy(str[i], tmp);
    }

    for(i=0;i<num;i++) {
        for(j=0;j<strlen(str[i]);j++) {
            if(str[i][j] == 'O' || str[i][j] == 'o'){
                total_score[i] += O_score;
                if(j != strlen(str[i])-1) {
                    if(str[i][j+1] == 'O' || str[i][j+1] == 'o')
                        O_score++;

                    else
                        O_score = 1;
                }
            }
            
            else if (str[i][j] == 'X' || str[i][j] == 'x')
                total_score[i] += X_SCORE;
        }

        O_score = 1;
    }

    for(i=0;i<num;i++) {
        printf("%d\n", total_score[i]);
    }

    for(i=0;i<num;i++)
        free(str[i]);

    free(str);
    free(total_score);
    exit(0);
}