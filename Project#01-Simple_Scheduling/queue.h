#pragma once

#include "common.h"
#include "process.h"

typedef struct Node{
    Process pcb;
    struct Node* next;
} Node;

typedef struct{
    Node* head;
    Node* tail;
    int count;
} Queue;

Queue* createQueue();
int isEmpty(Queue *q);
void enqueue(Queue *q, int pid, int cpu_burst, int io_burst, int remaining_time);
Process* dequeue(Queue *q);
void removeQueue(Queue *q);
void printQueue(const Queue *q, char type);
