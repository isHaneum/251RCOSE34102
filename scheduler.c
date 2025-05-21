#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"
#include "gantt.h"
#include "scheduler.h"

// ---------------- FCFS 스케줄링 구현 ----------------
void schedule_fcfs(Process procs[], int n, int max_time) {
    printf("\n--FCFS Scheduling--\n");
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;

    while (completed < n && time < max_time) {
        // 도착 프로세스 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_fcfs(&rq, &procs[i]);
                //printf("[Time %3d] %s arrived → Ready Queue\n", time, procs[i].id);
            }
        }
        // I/O 완료 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_fcfs(&rq, wp);
            //printf("[Time %3d] %s I/O complete → Ready Queue\n", time, wp->id);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
                //printf("[Time %3d] %s scheduled on CPU\n", time, running->id);
            }
        }
        // 실행 또는 idle
        if (running) {
            // 1ms 실행
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++;
            // 완료 시
            if (running->remaining_time == 0) {
                running->completed_time = time;
                //printf("[Time %3d] %s finished\n", time, running->id);
                log_execution(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            }
            // I/O 요청 (5% 확률)
            else if (running->executed_time > 1 && rand() % 20 == 0) {
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                //printf("[Time %3d] %s requests I/O for %d ms\n", time, running->id, io_burst);
                log_execution(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }

            
        } else {
            time++;
        }
    }
}

// ---------------- Round Robin 스케줄링 구현 ----------------
void schedule_rr(Process procs[], int n, int time_quantum, int max_time) {
    printf("\n--Round Robin Scheduling--\n");
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;
    int tq_counter = 0;

    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_fcfs(&rq, &procs[i]);
                //printf("[Time %3d] %s arrived → Ready Queue\n", time, procs[i].id);
            }
        }
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_fcfs(&rq, wp);
            //printf("[Time %3d] %s I/O complete → Ready Queue\n", time, wp->id);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                tq_counter = 0;
                segment_start = time;
                //printf("[Time %3d] %s scheduled on CPU\n", time, running->id);
            }
        }
        // 실행 또는 idle
        if (running) {
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++; tq_counter++;
            // I/O 요청
            if (running->remaining_time == 0 || tq_counter == time_quantum) {
                running->completed_time = time;
                bool io_flag = false;
                //printf("[Time %3d] %s %s\n", time, running->id,
                //       running->remaining_time == 0 ? "finished" : "quantum expired");
                log_execution(running->id, segment_start, time, io_flag);
                if (running->remaining_time > 0) ready_enqueue_fcfs(&rq, running);
                else completed++;
                running = NULL;
            }            
            else if (running->executed_time > 1 && rand() % 10000 == 0) {
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                printf("[Time %3d] %s requests I/O for %d ms\n", time, running->id, io_burst);
                log_execution(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }
            // time quantum 소진 또는 완료

        } 
        else  time++;

    }
}

// ---------------- SJF 스케줄링 구현 ----------------
void schedule_sjf(Process procs[], int n, int max_time) {
    printf("\n--SJF Scheduling--\n");
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;

    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_sjf(&rq, &procs[i]);
                //printf("[Time %3d] %s arrived → Ready Queue\n", time, procs[i].id);
            }
        }
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_sjf(&rq, wp);
            printf("[Time %3d] %s I/O complete → Ready Queue\n", time, wp->id);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
                //printf("[Time %3d] %s scheduled on CPU\n", time, running->id);
            }
        }
        // 실행 또는 idle
        if (running) {
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++;
            if (running->remaining_time == 0) {
                running->completed_time = time;
                //printf("[Time %3d] %s finished\n", time, running->id);
            
                log_execution(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            }// I/O 요청
            
            else if (running->executed_time > 1 && rand() % 20 == 0) {
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                printf("[Time %3d] %s requests I/O for %d ms\n", time, running->id, io_burst);
                log_execution(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }

            
        } else 
            // idle
            time++;
        
    }
}

void schedule_priority(Process procs[], int n, int max_time) {
    printf("\n---Priority Scheduling---\n");
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;

    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_priority(&rq, &procs[i]);
                //printf("[Time %3d] %s arrived → Ready Queue\n", time, procs[i].id);
            }
        }
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_priority(&rq, wp);
            //printf("[Time %3d] %s I/O complete → Ready Queue\n", time, wp->id);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
                //printf("[Time %3d] %s scheduled on CPU\n", time, running->id);
            }
        }
        // 실행 또는 idle
        if (running) {
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++;
            if (running->remaining_time == 0) {
                running->completed_time = time;
                //printf("[Time %3d] %s finished\n", time, running-> id);
                log_execution(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            }
            // I/O 요청
            else if (running->executed_time > 1 && rand() % 20 == 0) {
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                //printf("[Time %3d] %s requests I/O for %d ms\n", time, running->id, io_burst);
                log_execution(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }
            // 완료 시

        } else {
            // idle
            time++;
        }
    }
}

