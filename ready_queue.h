#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include <stdbool.h>
#include "process.h"

// ─── ReadyNode 선언 
typedef struct ReadyNode {
    Process*           process;
    struct ReadyNode*  prev;
    struct ReadyNode*  next;
} ReadyNode;

// ─── ReadyQueue 선언 
typedef struct {
    ReadyNode* head;
    ReadyNode* tail;
} ReadyQueue;

// 초기화
void init_ready_queue(ReadyQueue* q);

// 빈 큐 검사
bool is_ready_queue_empty(const ReadyQueue* q);

// FCFS: 맨 뒤에 삽입
void ready_enqueue_fcfs(ReadyQueue* q, Process* p);

// SJF: remaining_time 기준 오름차순 삽입
void ready_enqueue_sjf(ReadyQueue* q, Process* p);

// Priority: priority 기준 오름차순 삽입
void ready_enqueue_priority(ReadyQueue* q, Process* p);


// 맨 앞에서 제거하고 반환
Process* ready_dequeue(ReadyQueue* q);

// 큐에서 특정 프로세스 제거
bool ready_remove(ReadyQueue* q, Process* p);

#endif // READY_QUEUE_H