#include "scheduling.h"
#include "queue.h"
#include "log.h"

extern Queue *ready_queue;
extern Queue *wait_queue;

void initialize_scheduler(){
    ready_queue = createQueue();
    wait_queue = createQueue();
}

Process* schedule_next(){
    if(!isEmpty(ready_queue)){
        puts("HI");
        return dequeue(ready_queue);
    }
    return NULL;
}

void update_scheduler(Process *process){
    if(process->remaining_time > 0){
        enqueue(ready_queue, process->pid, process->remaining_time, process->io_burst);
        log_process_event(process, "Re-enqueued to READY queue");
    }
    else if(process->io_burst > 0){
        enqueue(wait_queue, process->pid, 0, process->io_burst);
        log_process_event(process, "Moved to WAIT queue for I/O");
    }
    else{
        log_process_event(process, "Completed");
    }
}

void terminate_scheduler(){
    removeQueue(ready_queue);
    removeQueue(wait_queue);
}
