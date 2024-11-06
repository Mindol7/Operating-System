#include "queue.h"

Queue* createQueue(){
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if(q == NULL){
        perror("Failed to create Queue");
        exit(EXIT_FAILURE);
    }

    q->head = q->tail = NULL;
    q->count = 0;
    return q;
}

int isEmpty(Queue *q){
    return (q->count == 0);
}

void enqueue(Queue *q, int idx, int cpu_burst, int io_burst){
    Node *node = (Node*)malloc(sizeof(Node));
    if(node == NULL){
        perror("Failed to enqueue process");
        exit(EXIT_FAILURE);
    }

    node->pcb.pid = idx;
    node->pcb.cpu_burst = cpu_burst;
    node->pcb.io_burst = io_burst;
    node->pcb.remaining_time = cpu_burst;
    node->next = NULL;

    if(q->tail == NULL){
        q->head = q->tail = node;
    }
    else{
        q->tail->next = node;
        q->tail = node;
    }
    q->count++;
}

Process* dequeue(Queue *q){
    if(isEmpty(q)){
        fprintf(stderr, "Queue is Empty\n");
        return NULL;
    }

    Node *temp = q->head;
    Process *process = &(temp->pcb);

    q->head = q->head->next;
    if(q->head == NULL){
        q->tail = NULL;
    }

    free(temp);
    q->count--;

    return process;
}

void removeQueue(Queue *q){
    while(!isEmpty(q)) dequeue(q);
    
    free(q);
}

void printQueue(const Queue *q, char type){
    Node *current = q->head;
    printf("Queue %c (Count: %d):\n", type, q->count);

    while(current != NULL){
        printf("Process ID: %d, CPU Burst: %d, IO Burst: %d\n",
                current->pcb.pid, current->pcb.cpu_burst, current->pcb.io_burst);
        
        current = current->next;
    }
    printf("\n");
}