#include "scheduling.h"
#include "queue.h"
#include "log.h"

extern Queue *ready_queue;
extern Queue *wait_queue;

void initialize_scheduler(){
    ready_queue = createQueue();
    wait_queue = createQueue();
}

void schedule(){
    if(!isEmpty(ready_queue)) {
        Node *current_ready = ready_queue->head;
        if(current_ready != NULL){

            if(current_ready->pcb.start_time == 0){
                current_ready->pcb.start_time = time_count;
            }

            if(current_ready->pcb.remaining_time == 0){ 
                if(current_ready->pcb.cpu_burst <= 0){
                    if(current_ready->pcb.io_burst <= 0){
                        update_process_state(current_ready, TERMINATED);
                        log_process_event(current_ready, "Completed");
                        
                        current_ready->pcb.completion_time = time_count;
                        turnaround_time += current_ready->pcb.completion_time - current_ready->pcb.arrival_time;
                        dequeue(ready_queue);
                    }
                    else{
                        log_process_event(current_ready, "Moved to WAIT queue for I/O");
                        enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst, time_quantum);
                        dequeue(ready_queue);
                    }
                }
                else{
                    log_process_event(current_ready, "Re-enqueued to Ready queue");
                    enqueue(ready_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, time_quantum);
                    dequeue(ready_queue);
                }
            }
            else if(current_ready->pcb.cpu_burst <= 0){
                if(current_ready->pcb.io_burst <= 0){
                    update_process_state(current_ready, TERMINATED);
                    log_process_event(current_ready, "Completed");
                    
                    current_ready->pcb.completion_time = time_count;
                    turnaround_time += current_ready->pcb.completion_time - current_ready->pcb.arrival_time;

                    dequeue(ready_queue);
                }
                else{
                    log_process_event(current_ready, "Re-enqueued to Wait queue");
                    enqueue(wait_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, time_quantum);
                    dequeue(ready_queue);
                }
            }
            else{
                log_process_event(current_ready, "Running");    
                current_ready->pcb.cpu_burst -= time_tick;
                current_ready->pcb.remaining_time -= time_tick;  
            }
        }
    }

    if(!isEmpty(wait_queue)){
        Node *current_wait = wait_queue->head;
        if(current_wait != NULL){
            if(current_wait->pcb.remaining_time == 0){
                if(current_wait->pcb.io_burst <= 0){
                    if(current_wait->pcb.cpu_burst <= 0){
                        update_process_state(current_wait, TERMINATED);
                        log_process_event(current_wait, "Completed");

                        current_wait->pcb.completion_time = time_count;
                        turnaround_time += current_wait->pcb.completion_time - current_wait->pcb.arrival_time;
                        
                        dequeue(wait_queue);
                    }
                    else{
                        log_process_event(current_wait, "Moved to Ready queue");
                        enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0, time_quantum);
                        dequeue(wait_queue);
                    }
                }
                else{
                    log_process_event(current_wait, "Re-enqueued to Wait queue");
                    enqueue(wait_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, current_wait->pcb.io_burst, time_quantum);
                    dequeue(wait_queue);
                }
            }
            else if(current_wait->pcb.io_burst <= 0){
                if(current_wait->pcb.cpu_burst <= 0){
                    update_process_state(current_wait, TERMINATED);
                    log_process_event(current_wait, "Completed");
                    
                    current_wait->pcb.completion_time = time_count;
                    turnaround_time += current_wait->pcb.completion_time - current_wait->pcb.arrival_time;

                    dequeue(wait_queue);
                }
                else{
                    log_process_event(current_wait, "Re-enqueued from Wait queue to Ready queue");
                    enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0, time_quantum);
                    dequeue(wait_queue);
                }
            }
            else{
                log_process_event(current_wait, "Running");    
                current_wait->pcb.io_burst -= time_tick;
                current_wait->pcb.remaining_time -= time_tick;
            }
        }
    }
}

void terminate_scheduler(){
    removeQueue(ready_queue);
    removeQueue(wait_queue);
}