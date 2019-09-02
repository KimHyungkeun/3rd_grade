#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/wait.h>

void ssu_do_cmd(char *cmd);
void ssu_print_times(clock_t real, struct tms *tms_start, struct tms *tms_end);
void ssu_echo_exit(int status);

int main(int argc, char* argv[]) {
    int i;

    setbuf(stdout, NULL);

    for (i = 1 ; i < argc ; i++) {
        ssu_do_cmd(argv[i]); //받아온 인자수만큼 ssu_do_cmd함수 실행
    }

    exit(0);
}

void ssu_do_cmd(char *cmd) {
    struct tms tms_start, tms_end;
    clock_t start, end;
    int status;

    printf("\ncommand: %s\n", cmd); //입력한 command을 실행

    if((start = times(&tms_start)) == -1) {
        fprintf(stderr, "times error\n"); // 타임함수 에러시 메세지 출력
        exit(1);
    }

    if((status = system(cmd)) < 0) {
        fprintf(stderr, "system error\n"); // 시스템 에러시 메세지 출력
        exit(1);
    }

    if((end = times(&tms_end)) == -1) {
        fprintf(stderr, "times error\n"); // 타임함수 에러시 출력
        exit(1);
    }

    ssu_print_times(end - start, &tms_start, &tms_end); //타임 출력 함수
    ssu_echo_exit(status); //종료상태 조사 

}

void ssu_print_times(clock_t real, struct tms *tms_start, struct tms *tms_end) {
        static long clocktick = 0;

        if(clocktick == 0)
            if((clocktick = sysconf(_SC_CLK_TCK)) < 0) {
                fprintf(stderr, "sysconf error\n");
                exit(1);
            }

        printf("    real: %7.2f\n", real / (double) clocktick); //실제
        printf("    user : %7.2f\n", (tms_end -> tms_utime - tms_start -> tms_utime) / (double) clocktick); //사용자 CPU
        printf("    sys : %7.2f\n", (tms_end -> tms_stime - tms_start -> tms_stime) / (double) clocktick); //시스템 CPU
        printf("    child user : %7.2f\n", (tms_end -> tms_cutime - tms_start -> tms_cutime) / (double) clocktick); //자식 사용자 CPU
        printf("    child sys : %7.2f\n", (tms_end -> tms_cstime - tms_start -> tms_cstime) / (double) clocktick); //자식 시스템 CPU
}

void ssu_echo_exit(int status) {
    if (WIFEXITED(status)) //자식프로세스 정상 종료의 경우
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //exit 인자에서 하위 8비트값 리턴
    else if(WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), //시그널 번호 리턴
        #ifdef WCOREDUMP
            WCOREDUMP(status) ? " (core file generated)" : ""); //코어 파일 생성의 경우 리턴
        #else
            "");
        #endif
            else if (WIFSTOPPED(status)) //자식 프로세스가 현재 중지 상태이면 참
                printf("child stopped, signal number = %d\n", WSTOPSIG(status)); //중지시킨 시그널 번호 리턴               
}
