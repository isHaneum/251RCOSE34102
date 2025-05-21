#include <stdlib.h>
#include "waiting_queue.h"

/**
 * I/O 대기 큐 초기화
 */
void init_waiting_queue(WaitingQueue* wq) {
    wq->front = NULL;
    wq->rear  = NULL;
}

/**
 * I/O 대기 큐에 프로세스 삽입 (enqueue)
 */
void waiting_enqueue(WaitingQueue* wq, Process* p) {
    WNode* node = (WNode*)malloc(sizeof(WNode));
    if (!node) return;  // 메모리 확보 실패 시 무시
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

/**
 * 대기 큐 맨 앞 프로세스 제거 (dequeue)
 */
void waiting_dequeue(WaitingQueue* wq) {
    if (!wq->front) return;
    WNode* tmp = wq->front;
    wq->front  = tmp->next;
    if (!wq->front) {
        wq->rear = NULL;
    }
    free(tmp);
}

