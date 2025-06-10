#ifndef TURNAROUND_TIME_H
#define TURNAROUND_TIME_H

#include "process.h"

// 프로세스 turnaroundtime 계산 
void print_turnaround_time(Process procs[], int n);

// 프로세스별 waitingtime 계산 및 출력
void print_waiting_time(Process procs[], int n);
float average_turnaround_time(Process procs[], int n);
float average_waiting_time(Process procs[], int n);

#endif // TURNAROUND_TIME_H
