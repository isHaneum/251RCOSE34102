#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

#define ID_LEN        12

//process 구조체
typedef struct {
    char id[ID_LEN];
    int  arrival_time;
    int  burst_time;
    int  remaining_time;
    int  io_burst_time;
    int  priority;
    int  executed_time;
    int  total_executed;
    int  io_complete_time;
    int  waiting_time;
    int  turnaround_time;
    int  completed_time; // 완료 시간
    int  io_burst_count;
} Process;


Process* create_random_process(const char* id, int pid);
void     create_processes(Process procs[], int n);
void     init_processes(Process procs[], int n);
void     print_process(const Process *p);
void     print_processes(const Process procs[], int n);

#endif // PROCESS_H
