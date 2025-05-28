// include/waiting_queue.h
#ifndef WAITING_QUEUE_H
#define WAITING_QUEUE_H

#include "process.h"

// I/O 대기 큐 node
typedef struct WNode {
    Process* p;
    struct WNode* next;
} WNode;

// I/O 대기 큐
typedef struct {
    WNode* front;
    WNode* rear;
} WaitingQueue;


// 큐 초기화
void init_waiting_queue(WaitingQueue* wq);
// 대기 큐에 삽입
void waiting_enqueue(WaitingQueue* wq, Process* p);
// 대기 큐 맨 앞 프로세스 확인
Process* waiting_peek(WaitingQueue* wq);
// 대기 큐 맨 앞 프로세스 제거
void waiting_dequeue(WaitingQueue* wq);

#endif // WAITING_QUEUE_H


// include/scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "ready_queue.h"
#include "waiting_queue.h"

// FCFS
void schedule_fcfs(Process procs[], int n, int max_time);

// Round Robin
void schedule_rr(Process procs[], int n, int time_quantum, int max_time);

#endif // SCHEDULER_H
