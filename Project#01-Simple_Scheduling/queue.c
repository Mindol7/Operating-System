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

void enqueue(Queue *q, int pid, int cpu_burst, int io_burst){
    Node *node = (Node*)malloc(sizeof(Node));

    if(node == NULL){
        perror("Failed to enqueue process");
        exit(EXIT_FAILURE);
    }

    node->pcb.pid = pid;
    node->pcb.cpu_burst = cpu_burst;
    node->pcb.io_burst = io_burst;
    node->next = NULL;

    printf("ENQUEUE PID DEBUG: %d\n", pid);
    printf("ENQUEUE DEBUG: %d\n", node->pcb.pid);

    if(q->head == NULL){
        q->head = node;
        q->tail = node;
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

    if(!isEmpty(q)){
        Node *temp = q->head;
        
        Process *process = (Process*)malloc(sizeof(Process));
        if (temp != NULL) {
            if (process == NULL) {
                perror("Failed to allocate memory for process");
                exit(EXIT_FAILURE);
            }
            
            *process = temp->pcb;  // Process 복사
            
            // Node 메모리 해제
            printf("temp Freeing complete I/O process with PID: %d\n", temp->pcb.pid);
            free(temp);
            temp = NULL;
        }
        if (q->head->next == NULL) {
            q->head == NULL;
            q->tail == NULL;
        }
        else{
            q->head = q->head->next;
        }
        
        q->count--;

        return process;  // 반환된 process는 한 번만 free 되어야 함
    }

    
}

void removeQueue(Queue *q){
    while(!isEmpty(q)){
        dequeue(q);
    }
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
