#pragma once

#include "common.h"
#include "process.h"

typedef struct Node{
    Process pcb;
    struct Node* next;
} Node;

typedef struct{
    Node* head; // 큐의 첫 번째 노드 가르킴
    Node* tail; // 큐의 마지막 가르킴
    int count; // 큐에 있는 노드 갯수
}Queue;

Queue* createQueue();

int isEmpty(Queue *q);

void enqueue(Queue *q, int idx, int cpu_burst, int io_burst);

Process* dequeue(Queue *q);

void removeQueue(Queue *q);

void printQueue(const Queue *q, char type);