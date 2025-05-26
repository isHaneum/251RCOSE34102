#include <stdlib.h>
#include "waiting_queue.h"

//io queue 정의의
void init_waiting_queue(WaitingQueue* wq) {
    wq->front = NULL;
    wq->rear  = NULL;
}

//enqueue 
void waiting_enqueue(WaitingQueue* wq, Process* p) {
    WNode* node = (WNode*)malloc(sizeof(WNode));
    if (!node) return;  // null시 무시
    node->p    = p;
    node->next = NULL;

    if (wq->rear) {
        wq->rear->next = node;
    } else {
        wq->front = node;
    }
    wq->rear = node;
}

Process* waiting_peek(WaitingQueue* wq) {
    return wq->front ? wq->front->p : NULL;
}

//dequeue
void waiting_dequeue(WaitingQueue* wq) {
    if (!wq->front) return;
    WNode* tmp = wq->front;
    wq->front  = tmp->next;
    if (!wq->front) {
        wq->rear = NULL;
    }
    free(tmp);
}

