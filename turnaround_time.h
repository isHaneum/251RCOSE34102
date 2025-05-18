#ifndef TURNAROUND_TIME_H
#define TURNAROUND_TIME_H

#include "process.h"

// 각 프로세스별 turnaround time 계산 및 출력
void print_turnaround_time(Process procs[], int n);

// 각 프로세스별 waiting time 계산 및 출력
void print_waiting_time(Process procs[], int n);

#endif // TURNAROUND_TIME_H
