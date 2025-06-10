#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include <stdbool.h>
#include "process.h"

// ─── ReadyNode
typedef struct ReadyNode {
    Process*           process;
    struct ReadyNode*  prev;
    struct ReadyNode*  next;
} ReadyNode;

// ─── ReadyQueue 
typedef struct {
    ReadyNode* head;
    ReadyNode* tail;
} ReadyQueue;

// 초기화
void init_ready_queue(ReadyQueue* q);

//
bool is_ready_queue_empty(const ReadyQueue* q);

//FCFS 맨 뒤에 삽입
void ready_enqueue_fcfs(ReadyQueue* q, Process* p);

//SJF remainingtime 기준 삽입
void ready_enqueue_sjf(ReadyQueue* q, Process* p);

//priority 기준 삽입
void ready_enqueue_priority(ReadyQueue* q, Process* p);


// 맨 앞에서 제거하고 반환
Process* ready_dequeue(ReadyQueue* q);



#endif // READY_QUEUE_H
