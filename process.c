#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   // for rand()

// 랜덤 프로세스 생성: 동적 할당 후 필드 초기화
Process* create_random_process(const char* id, int pid) {
    Process* p = malloc(sizeof(Process)); //p에 동적 할당
    if (!p) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memset(p, 0, sizeof(*p)); 

    strncpy(p->id, id, ID_LEN-1);
    p->id[ID_LEN-1] = '\0';

    p->arrival_time     = rand() % 16;         // 도착 시간 랜덤 0~15
    p->burst_time       = rand() % 8 + 2;      // burst 시간 랜덤 2~9
    p->io_burst_time    = rand() % 3 + 2;      // io burst 시간 랜덤 2~4
    p->remaining_time   = p->burst_time;       // burst 후 남은 bursttime
    p->priority         = rand() % 8;          // 우선 순위 랜덤 0~7 
    p->executed_time    = 0;                   // 실행된 순간부터 시간, 재실행시 초기화
    p->total_executed   = 0;                   // 전부 얼마나 실행 됐는지?
    p->io_complete_time = 0;                   // io 완료 시간
    p->waiting_time     = 0;                   // waiting time == completed_time - arrival_time - burst_time
    p->turnaround_time  = 0;                   // turnaround time == wating_time + burst_time
    p -> completed_time = 0;                   // process 완료 시간

    return p;
}

//processes 생성
void create_processes(Process procs[], int n) {
    for (int i = 0; i < n; i++) {
        char id[ID_LEN];
        snprintf(id, ID_LEN, "P%d", i+1);
        Process* tmp = create_random_process(id, i+1);
        procs[i] = *tmp;
        free(tmp);
    }
}

//process reset
void init_processes(Process procs[], int n) {
    for (int i = 0; i < n; i++) {
        procs[i].remaining_time   = procs[i].burst_time;
        procs[i].executed_time    = 0;
        procs[i].total_executed   = 0;
        procs[i].io_complete_time = 0;
        procs[i].waiting_time     = 0;
        procs[i].turnaround_time  = 0;
    }
}

//process 정보 print
void print_process(const Process *p) {
    printf("ID=%s | arrival=%2d | burst=%2d | prio=%2d | io_burst=%2d\n",
           p->id, p->arrival_time, p->burst_time, p->priority, p->io_burst_time);//5개만 출력
}

//process 전부 출력
void print_processes(const Process procs[], int n) {
    printf("=== Generated Processes ===\n");
    for (int i = 0; i < n; i++) {
        print_process(&procs[i]);
    }
    printf("===========================\n");
}
