#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 1024

char path[] = "STD_DIR/20190001/";
char path_backup[] = "STD_DIR/20190001/";


int main(void)
{
    struct  dirent **namelist;
    int     count, fd_count;
    int     idx;
    int     fd;
    char buf[BUFFER_SIZE];
    
    if((count = scandir(path, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", path, strerror(errno));
        exit(1);
    }

    

    for(idx = 0; idx < count; idx++) {
        if(strcmp(namelist[idx] -> d_name,".") == 0 || strcmp(namelist[idx] -> d_name,"..") == 0)
        continue;

        else {
        printf("%s\n", namelist[idx]->d_name);
        strcat(path, namelist[idx] -> d_name);
        fd = open(path, O_RDONLY);
        fd_count = read(fd, buf, BUFFER_SIZE); //처음부터 시작하여 1024byte 만큼을 읽는다.
        printf("%d bytes\n", fd_count);
		buf[fd_count] = 0;
        strcpy(path, path_backup);
        close(fd);
        }
    }

    


    // �Ǻ� ������ �޸� ����
    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    // namelist�� ���� �޸� ����
    free(namelist);

    return 0;
}
