
// include/scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"

// FCFS 스케줄링 실행
// max_time: 시뮬레이션 최대 시간(ms)
void schedule_fcfs(Process procs[], int n, int max_time);

// Round Robin 스케줄링 실행
// time_quantum: 타임 퀀텀(ms)
void schedule_rr(Process procs[], int n, int time_quantum, int max_time);

void schedule_sjf(Process procs[], int n, int max_time);

void schedule_priority(Process procs[], int n, int max_time)


#endif // SCHEDULER_H