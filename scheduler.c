#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"
#include "gantt.h"
#include "scheduler.h"

// ---------------- FCFS 스케줄링 ----------------
void schedule_fcfs(Process procs[], int n, int max_time) {
    printf("\n--FCFS Scheduling--\n");
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;//완료된 갯수
    int time = 0;//cpu시간
    Process* running = NULL;
    int segment_start = -1;// 임의 process 시작시간
    int idle_time = 0; // idle 시간 추적
    while (completed < n && time < max_time) {
        // 도착 프로세스 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_fcfs(&rq, &procs[i]);
            }//들어올때마다 확인해서 fcfs queue에 넣기
        }
        // I/O 완료 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_fcfs(&rq, wp);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
            }
        }
        // 실행 또는 idle
        if (running) {
            // 1ms 실행
            if (idle_time > 0) {//이전에 idle 했었다면 gantt에 idle 추가, idletime 초기화
                log_store("IDLE", time - idle_time, time, false);
                idle_time = 0;
            }
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++; //
            time++;
            // 완료 시
            if (running->remaining_time == 0) {
                running->completed_time = time;
                log_store(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            }
            // I/O 요청 (2% 확률)
            else if (running->executed_time >= 1 && rand() % 25 == 0) { //1초라도 실행해야함
                running->io_burst_count += 1; // io burst 횟수 증가
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                log_store(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }

        }    
        else {//할당이 안됐다면 idle
                idle_time++;
                time++;
                continue;
        }
    }
}

// ---------------- Round Robin 스케줄링 ----------------
void schedule_rr(Process procs[], int n, int time_quantum, int max_time) {
    printf("\n--Round Robin Scheduling-- (quantum: %d)\n", time_quantum);
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;
    int tq_counter = 0;
    int idle_time = 0; // idle 시간 추적
    
    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_fcfs(&rq, &procs[i]);
            }
        }
        
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_fcfs(&rq, wp);
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
            }
        }
        // 실행 또는 idle
        if (running) {
            if (idle_time > 0) {
                log_store("IDLE", time - idle_time, time, false);
                idle_time = 0;
                
            }
            
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++; 
            tq_counter++;
            // 완료 or time quantum 끝
 if ((running->remaining_time == 0) || (tq_counter >= time_quantum && !is_ready_queue_empty(&rq))) {
            log_store(running->id, segment_start, time, false);
            if ((running->remaining_time == 0)) {
                running->completed_time = time;
                completed++;
            }
            else 
            {
                ready_enqueue_fcfs(&rq, running);
            }
            running = NULL;
            continue;
            }            
            else if (running->executed_time >= 1 && rand() % 50 == 0) {

                running->io_burst_count++; // io burst 횟수 증가
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                log_store(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }

        } 

        
        else {
                idle_time++;
                time++;
                continue;
        }
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
    int idle_time = 0;
    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_sjf(&rq, &procs[i]);
            }
        }
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_sjf(&rq, wp);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
            }
        }
        // 실행 or idle
        if (running) {
            if (idle_time > 0) {
                log_store("IDLE", time - idle_time, time, false);
                idle_time = 0;
            }
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++;
            if (running->remaining_time == 0) {
                running->completed_time = time;
            
                log_store(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            }
            // I/O
            else if (running->executed_time >= 1 && rand() % 50 == 0) {
                running->io_burst_count++; // io burst 횟수 증가
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                log_store(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }

            
        }
        else {
                idle_time++;
                time++;
                continue;
        }
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
    int idle_time = 0;
    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_priority(&rq, &procs[i]);
            }
        }
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_priority(&rq, wp);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
            }
        }
        // 실행 또는 idle
        if (running) {
            if (idle_time > 0) {
                log_store("IDLE", time - idle_time, time, false);
                idle_time = 0;
            }
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++;
            if (running->remaining_time == 0) {
                running->completed_time = time;
                log_store(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            }
            // I/O 요청
            else if (running->executed_time >= 1 && rand() % 50 == 0) {
                running->io_burst_count++; // io burst 횟수 증가
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                log_store(running->id, segment_start, time, true);//io==true
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }
            // 완료 시

        } 
        else {
                idle_time++;
                time++;
                continue;
        }
    }
}

void preemptive_sjf(Process procs[], int n, int max_time) {
    printf("\n--Preemptive SJF Scheduling--\n");
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;
    int idle_time = 0;

    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_sjf(&rq, &procs[i]);
            }
        }
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_sjf(&rq, wp);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }

        // SJF preemption
        if (running) {
            Process* next = ready_dequeue(&rq);
            if (next && next->remaining_time < running->remaining_time) {
                log_store(running->id, segment_start, time, false);
                ready_enqueue_sjf(&rq, running);
                running = next;
                running->executed_time = 0;
                segment_start = time;
            }
            else if (next) {
                ready_enqueue_sjf(&rq, next); // 다시 큐에 넣기
            }
        }

        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
            }
        }

        // 실행 or idle
        if (running) {
            if (idle_time > 0) {
                log_store("IDLE", time - idle_time, time, false);
                idle_time = 0;
            }
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++;

            // 완료 시
            if (running->remaining_time == 0) {
                running->completed_time = time;
                log_store(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            } 
            // I/O 요청
            else if (running->executed_time >= 1 && rand() % 50 == 0) {
                
                running->io_burst_count++; // io burst 횟수 증가
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                
                log_store(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }
        }
        else {
            idle_time++;
            time++;
            continue;
        }
    }
}


void preemptive_priority(Process procs[], int n, int max_time) {
    printf("\n--Preemptive Priority Scheduling--\n");
    ReadyQueue rq; init_ready_queue(&rq);
    WaitingQueue wq; init_waiting_queue(&wq);
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;
    int idle_time = 0;
    
    while (completed < n && time < max_time) {
        // 도착 enqueue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                ready_enqueue_priority(&rq, &procs[i]);
            }
        }
        // I/O 복귀
        Process* wp = waiting_peek(&wq);
        while (wp && wp->io_complete_time <= time) {
            ready_enqueue_priority(&rq, wp);
            waiting_dequeue(&wq);
            wp = waiting_peek(&wq);
        }
        // CPU 할당
        if (!running) {
            running = ready_dequeue(&rq);
            if (running) {
                running->executed_time = 0;
                segment_start = time;
            }
        }
        // 실행 또는 idle
        if (running) {
            if (idle_time > 0) {
                log_store("IDLE", time - idle_time, time, false);
                idle_time = 0;
            }
            Process* next = ready_dequeue(&rq);
            if (next && next->priority < running->priority) {
                log_store(running->id, segment_start, time, false);
                ready_enqueue_priority(&rq, running);

                running = next;
                running->executed_time = 0;
                segment_start = time;

            }
            else if (next) {
            ready_enqueue_priority(&rq, next); // 다시 큐에 넣기
            }
            running->remaining_time--;
            running->executed_time++;
            running->total_executed++;
            time++;

            if (running->remaining_time == 0) {
                running->completed_time = time;
                log_store(running->id, segment_start, time, false);
                completed++;
                running = NULL;
            }
            // I/O 요청
            else if (running->executed_time >= 1 && rand() % 50 == 0) {
                running->io_burst_count++; // io burst 횟수 증가
                int io_burst = running->io_burst_time;
                running->io_complete_time = time + io_burst;
                log_store(running->id, segment_start, time, true);
                waiting_enqueue(&wq, running);
                running = NULL;
                continue;
            }

            // Priority Preemption

            
        } 
        else {
            idle_time++;
            time++;
            continue;
        }
    }
}
