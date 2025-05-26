
//scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H
//process, ready_queue, waiting_queue 이용
#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"

// FCFS
void schedule_fcfs(Process procs[], int , int );

// Round Robin
void schedule_rr(Process procs[], int , int , int );
// shortjob first
void schedule_sjf(Process procs[], int , int );
// prioriry
void schedule_priority(Process procs[], int , int );

void preemptive_sjf(Process procs[], int n, int max_time);

void preemptive_priority(Process procs[], int n, int max_time);
#endif // SCHEDULER_H
