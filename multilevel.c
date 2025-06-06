#include "multilevel.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"
#include "gantt.h"
#include "scheduler.h"

void schedule_mlq(Process procs[], int n, int max_time) {
    ReadyQueue first_q, second_q, third_q;
    init_ready_queue(&first_q);
    init_ready_queue(&second_q);
    init_ready_queue(&third_q);
    printf("\n--Multilevel Queue Scheduling--\n");
    printf("First Queue (RR = 4): Priority 0-2\n");
    printf("Second Queue (FCFS): Priority 3-5\n");
    printf("Third Queue (SJF): Priority 6-7\n");
    
    int completed = 0;
    int time = 0;
    Process* running = NULL;
    int segment_start = -1;
    int tq_counter = 0;
    const int time_quantum = 4; // rr 4임의
    int queue_num = -1;
    int idle_time = 0; // idle 시간 추적

    while (completed < n && time < max_time) {
        // 분류
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival_time == time) {
                if (procs[i].priority < 3) {
                    ready_enqueue_fcfs(&first_q, &procs[i]);  // RR 큐
                } else if (procs[i].priority < 6) {
                    ready_enqueue_fcfs(&second_q, &procs[i]); // FCFS 큐
                } else {
                    ready_enqueue_sjf(&third_q, &procs[i]);   // SJF 큐
                }
            }
        }

        // CPU 비어있을 때
        if (!running) {
            if (!is_ready_queue_empty(&first_q)) 
            {
                running = ready_dequeue(&first_q);
                queue_num = 0;
                tq_counter = 0;
                segment_start = time;
            } 
            else if (!is_ready_queue_empty(&second_q)) 
            {
                running = ready_dequeue(&second_q);
                queue_num = 1;
                segment_start = time;
                
            } 
            else if (!is_ready_queue_empty(&third_q))
            {
                running = ready_dequeue(&third_q);
                queue_num = 2;
                segment_start = time;

            }

            if (running) {
                // 작업 시작
                if (idle_time > 0) {
                    log_execution("IDLE", time - idle_time, time, false);
                }
                idle_time = 0; // idle 시간 초기화
            } 
            else
            {
                // 모든 큐 비어있으면 idle 기록
                idle_time++;
                time++;
                continue;
            }
        }

        // 실행: 1 tick
        running->remaining_time--;
        running->executed_time++;
        running->total_executed++;
        tq_counter++;
        time++;

        // 종료 or rr = 0


        if ((running->remaining_time == 0) || (queue_num == 0 && tq_counter >= time_quantum && !is_ready_queue_empty(&first_q))) {
            log_execution(running->id, segment_start, time, false);
            if ((running->remaining_time == 0)) {
                running->completed_time = time;
                completed++;
            }
            else 
            {
                ready_enqueue_fcfs(&first_q, running);
            }
            running = NULL;
            continue;
        }
    }

    // 결과 출력 및 정리
    print_gantt_chart();
    print_turnaround_time(procs, n);
    print_waiting_time(procs, n);
    init_processes(procs, n);
    clear_gantt_log();
}
    
