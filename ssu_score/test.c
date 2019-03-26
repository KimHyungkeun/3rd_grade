#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

void *ssu_thread(void *arg) {
    
    pthread_t tid;
    pid_t pid;


    pid = getpid();
    tid = pthread_self();
    printf("New Thread : pid %d tid %u \n",(int)pid, (unsigned int)tid);
    return NULL;

}

int main (void) {

    time_t first, second;
    pthread_t tid;
    pid_t pid;

    
    while(1) {
     if(pthread_create(&tid, NULL, ssu_thread, NULL) != 0) {
        fprintf(stderr, "pthread_creat error\n");
        exit(1);
    }

    

    pid = getpid();
    tid = pthread_self();
    
    printf("Main Thread : pid %d tid %u \n",(int)pid, (unsigned int)tid);
    sleep(1);

    
    if( (second = time(NULL)) > 0 ) {
        printf("%ld sec\n", second);
        break;
    }
    }
    

    exit(0);
    
}

