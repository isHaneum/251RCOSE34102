#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"  // Process.h에서 

//Ready Queue (Doubly Linked List)
typedef struct ReadyNode {
    Process* process;
    struct ReadyNode* prev; // 이전 노드 포인터
    struct ReadyNode* next; // 다음 노드 포인터
} ReadyNode;

typedef struct { // ReadyQueue struct
    ReadyNode* head;
    ReadyNode* tail;
} ReadyQueue;

// 초기화
void init_ready_queue(ReadyQueue* q) {
    q->head = q->tail = NULL;
}

// 빈 큐 검사
bool is_ready_queue_empty(ReadyQueue* q) {
    return q->head == NULL;
}

// FCFS용 queue 맨 뒤에 삽입
void ready_enqueue_fcfs(ReadyQueue* q, Process* p) {//온 순서대로 삽입
    ReadyNode* node = (ReadyNode*)malloc(sizeof(ReadyNode));
    node->process = p;
    node->prev = q->tail;
    node->next = NULL;

    if (q->tail) {
        q->tail->next = node;
    } else {
        q->head = node;
    }
    q->tail = node;
}

// SJF용 remaining_time 기준 삽입
void ready_enqueue_sjf(ReadyQueue* q, Process* p) {
    ReadyNode* node = (ReadyNode*)malloc(sizeof(ReadyNode));
    node->process = p;
    node->prev = node->next = NULL;

    if (q->head == NULL) {// 
        q->head = q->tail = node;
        return;
    }
    ReadyNode* cur = q->head;
    while (cur && cur->process->remaining_time <= p->remaining_time) {//remaining_time 기준 오름차
        cur = cur->next;
    }
    if (cur == NULL) {
        // tail 뒤에 삽입
        node->prev = q->tail;
        q->tail->next = node;
        q->tail = node;
    } else {
        // 중간 또는 head 앞 삽입
        node->next = cur;
        node->prev = cur->prev;
        if (cur->prev) cur->prev->next = node;
        else q->head = node;
        cur->prev = node;
    }
}

// Priority: priority 기준 오름차순 삽입
void ready_enqueue_priority(ReadyQueue* q, Process* p) {
    ReadyNode* node = (ReadyNode*)malloc(sizeof(ReadyNode));
    node->process = p;
    node->prev = node->next = NULL;

    if (q->head == NULL) {
        q->head = q->tail = node;
        return;
    }
    ReadyNode* cur = q->head;
    while (cur && cur->process->priority <= p->priority) {
        cur = cur->next;
    }
    if (cur == NULL) {
        node->prev = q->tail;
        q->tail->next = node;
        q->tail = node;
    } else {
        node->next = cur;
        node->prev = cur->prev;
        if (cur->prev) cur->prev->next = node;
        else q->head = node;
        cur->prev = node;
    }
}


Process* ready_dequeue(ReadyQueue* q) {
    if (q->head == NULL) return NULL;
    ReadyNode* node = q->head;
    Process* p = node->process;

    q->head = node->next;
    if (q->head) q->head->prev = NULL;
    else q->tail = NULL;
    free(node);
    return p;

}
bool ready_remove(ReadyQueue* q, Process* p) {
    ReadyNode* cur = q->head;
    while (cur) {
        if (cur->process == p) {
            if (cur->prev) cur->prev->next = cur->next;
            else q->head = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            else q->tail = cur->prev;
            free(cur);
            return true;
        }
        cur = cur->next;
    }
    return false;
}
