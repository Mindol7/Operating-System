#include "scheduling.h"
#include "queue.h"
#include "log.h"

extern Queue *ready_queue;

void initialize_scheduler(){
    ready_queue = createQueue();
}

Process* schedule_next(){
    Process *process = dequeue(ready_queue);
}

void update_scheduler(Process *process){
    if(process->remaining_time > 0){
        enqueue(ready_queue, process->pid, process->cpu_burst, process->io_burst);
        log_process_event(process, "Re-enqueued to READY queue");
    }
    else{
        log_process_event(process, "Completed");
    }
}

void terminate_scheduler(){
    removeQueue(ready_queue);
}