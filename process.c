#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   // for rand()

/**
 * 랜덤 프로세스 생성: 동적 할당 후 필드 초기화
 */
Process* create_random_process(const char* id, int pid) {
    Process* p = malloc(sizeof(Process));
    if (!p) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memset(p, 0, sizeof(*p));

    strncpy(p->id, id, ID_LEN-1);
    p->id[ID_LEN-1] = '\0';

    p->arrival_time     = rand() % 10;         // 0~9
    p->burst_time       = rand() % 8 + 2;      // 2~9
    p->io_burst_time    = rand() % 3 + 2;      // 2~4
    p->remaining_time   = p->burst_time;
    p->priority         = rand() % 8;          // 0~7
    p->executed_time    = 0;
    p->total_executed   = 0;
    p->io_complete_time = 0;
    p->waiting_time     = 0;
    p->turnaround_time  = 0;
    p -> completed_time = 0;

    return p;
}

/**
 * 프로세스 배열 생성
 */
void create_processes(Process procs[], int n) {
    if (n > MAX_PROCESSES) n = MAX_PROCESSES;
    for (int i = 0; i < n; i++) {
        char id[ID_LEN];
        snprintf(id, ID_LEN, "P%d", i+1);
        Process* tmp = create_random_process(id, i+1);
        procs[i] = *tmp;
        free(tmp);
    }
}

/**
 * 프로세스 초기화
 */
void init_processes(Process procs[], int n) {
    if (n > MAX_PROCESSES) n = MAX_PROCESSES;
    for (int i = 0; i < n; i++) {
        procs[i].remaining_time   = procs[i].burst_time;
        procs[i].executed_time    = 0;
        procs[i].total_executed   = 0;
        procs[i].io_complete_time = 0;
        procs[i].waiting_time     = 0;
        procs[i].turnaround_time  = 0;
    }
}

/**
 * 단일 프로세스 정보 출력
 */
void print_process(const Process *p) {
    printf("ID=%s | arrival=%2d | burst=%2d | prio=%2d | io_burst=%2d\n",
           p->id, p->arrival_time, p->burst_time, p->priority, p->io_burst_time);
}

/**
 * 프로세스 배열 전체 정보 출력
 */
void print_processes(const Process procs[], int n) {
    if (n > MAX_PROCESSES) n = MAX_PROCESSES;
    printf("=== Generated Processes ===\n");
    for (int i = 0; i < n; i++) {
        print_process(&procs[i]);
    }
    printf("===========================\n");
}