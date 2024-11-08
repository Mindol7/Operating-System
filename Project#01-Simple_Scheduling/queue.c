#include "queue.h"

Queue* createQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        perror("Failed to create Queue");
        exit(EXIT_FAILURE);
    }

    q->head = q->tail = NULL;
    q->count = 0;
    return q;
}

int isEmpty(Queue *q) {
    return (q->count == 0);
}

void enqueue(Queue *q, int pid, int cpu_burst, int io_burst, int remaining_time) {
    Node *node = (Node*)malloc(sizeof(Node));

    if (node == NULL) {
        perror("Failed to enqueue process");
        exit(EXIT_FAILURE);
    }

    node->pcb.pid = pid;
    node->pcb.cpu_burst = cpu_burst;
    node->pcb.io_burst = io_burst;
    node->pcb.remaining_time = remaining_time;
    node->next = NULL;

    if (q->head == NULL) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->count++;
}

Process* dequeue(Queue *q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue is Empty\n");
        return NULL;
    }

    Node *temp = q->head;
    Process *process = (Process*)malloc(sizeof(Process));
    if (process == NULL) {
        perror("Failed to allocate memory for process");
        exit(EXIT_FAILURE);
    }
    *process = temp->pcb;

    q->head = q->head->next;  
    if (q->head == NULL) {     
        q->tail = NULL;
    }

    free(temp);  
    q->count--;

    return process;
}

void removeQueue(Queue *q) {
    while (!isEmpty(q)) {
        Process *p = dequeue(q);
        free(p); 
    }
    free(q);
}