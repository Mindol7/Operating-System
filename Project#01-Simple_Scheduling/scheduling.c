#include "scheduling.h"
#include "queue.h"
#include "log.h"

extern Queue *ready_queue;
extern Queue *wait_queue;

int rev = 0;

void initialize_scheduler(){
    ready_queue = createQueue();
    wait_queue = createQueue();
}
void FCFS_schedule(){
    if(!isEmpty(ready_queue)){
        Node *current_ready = ready_queue->head;
        if(current_ready != NULL){
            if(current_ready->pcb.flag == 0){
                current_ready->pcb.flag = 1;
                rev++;
                current_ready->pcb.start_time = time_count - rev;
                wait_time = current_ready->pcb.start_time;
                total_wait_time += wait_time;
            }
 
            if(current_ready->pcb.cpu_burst <= 0){ // cpu burst 끝나면 끝내거나 io 요청
                if(current_ready->pcb.io_burst <= 0){
                    update_process_state(current_ready, TERMINATED);
                    log_process_event(current_ready, "Completed");
                    
                    current_ready->pcb.completion_time = time_count;
                    dequeue(ready_queue);
                }
                else{
                    log_process_event(current_ready, "Moved to WAIT queue for I/O");
                    enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst, current_ready->pcb.arrival_time, time_quantum);
                    
                    current_ready->pcb.completion_time = time_count;
                    dequeue(ready_queue);
                }
            }        
            else{
                log_process_event(current_ready, "Running");    
                current_ready->pcb.cpu_burst--; 
            }
        }        
    }

    if(!isEmpty(wait_queue)){
        Node *current_wait = wait_queue->head;
        if(current_wait != NULL){
            if(current_wait->pcb.io_burst <= 0){
                if(current_wait->pcb.cpu_burst <= 0){
                    update_process_state(current_wait, TERMINATED);
                    log_process_event(current_wait, "Completed");
                    turnaround_time += time_count;
                    dequeue(wait_queue);
                }
            }   
            else{
                log_process_event(current_wait, "Running");    
                current_wait->pcb.io_burst--;
            }
        }
    }
}

void RR_schedule(){
    if(!isEmpty(ready_queue)) {
        Node *current_ready = ready_queue->head;
        if(current_ready != NULL){
            if(current_ready->pcb.flag == 0){
                current_ready->pcb.flag = 1;
                rev++;
                current_ready->pcb.start_time = time_count - rev;
                wait_time = current_ready->pcb.start_time;
                total_wait_time += wait_time;
            }

            if(current_ready->pcb.remaining_time == 0){ 
                if(current_ready->pcb.cpu_burst <= 0){
                    if(current_ready->pcb.io_burst <= 0){
                        update_process_state(current_ready, TERMINATED);
                        log_process_event(current_ready, "Completed");
                                
                        current_ready->pcb.completion_time = time_count;
                    wait_time += (current_ready->pcb.completion_time);
                        dequeue(ready_queue);
                    }
                    else{
                        log_process_event(current_ready, "Moved to WAIT queue for I/O");
                        enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst, current_ready->pcb.arrival_time, time_quantum);
                        dequeue(ready_queue);
                    }
                }
                else{
                    log_process_event(current_ready, "Re-enqueued to Ready queue");
                    enqueue(ready_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, current_ready->pcb.arrival_time, time_quantum);
                    dequeue(ready_queue);
                }
            }
            else if(current_ready->pcb.cpu_burst <= 0){
                if(current_ready->pcb.io_burst <= 0){
                    update_process_state(current_ready, TERMINATED);
                    log_process_event(current_ready, "Completed");
                    
                    current_ready->pcb.completion_time = time_count;
                    wait_time += (current_ready->pcb.completion_time);

                    dequeue(ready_queue);
                }
                else{
                    log_process_event(current_ready, "Re-enqueued to Wait queue");
                    enqueue(wait_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, current_ready->pcb.arrival_time, time_quantum);
                    dequeue(ready_queue);
                }
            }
            else{
                log_process_event(current_ready, "Running");    
                current_ready->pcb.cpu_burst--;
                current_ready->pcb.remaining_time--;  
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
                        turnaround_time += time_count;
                        
                        dequeue(wait_queue);
                    }
                    else{
                        log_process_event(current_wait, "Moved to Ready queue");
                        enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0, current_wait->pcb.arrival_time, time_quantum);
                        dequeue(wait_queue);
                    }
                }
                else{
                    log_process_event(current_wait, "Re-enqueued to Wait queue");
                    enqueue(wait_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, current_wait->pcb.io_burst, current_wait->pcb.arrival_time, time_quantum);
                    dequeue(wait_queue);
                }
            }
            else if(current_wait->pcb.io_burst <= 0){
                if(current_wait->pcb.cpu_burst <= 0){
                    update_process_state(current_wait, TERMINATED);
                    log_process_event(current_wait, "Completed");
                    
                    turnaround_time += time_count;

                    dequeue(wait_queue);
                }
                else{
                    log_process_event(current_wait, "Re-enqueued from Wait queue to Ready queue");
                    enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0, current_wait->pcb.arrival_time, time_quantum);
                    dequeue(wait_queue);
                }
            }
            else{
                log_process_event(current_wait, "Running");    
                current_wait->pcb.io_burst--;
                current_wait->pcb.remaining_time--;
            }
        }
    }
}

void SJF_schedule(){
    if(!isEmpty(ready_queue)) {
        Node *current_ready = ready_queue->head;
        if(current_ready != NULL){
            if(current_ready->pcb.flag == 0){
                current_ready->pcb.flag = 1;
                
                current_ready->pcb.start_time = time_count - rev;
                wait_time = current_ready->pcb.start_time;
                total_wait_time += wait_time;
                rev++;
            }
 
            if(current_ready->pcb.cpu_burst <= 0){ // cpu burst 끝나면 끝내거나 io 요청
                if(current_ready->pcb.io_burst <= 0){
                    update_process_state(current_ready, TERMINATED);
                    log_process_event(current_ready, "Completed");
                        
                    current_ready->pcb.completion_time = time_count;
                    wait_time += (current_ready->pcb.completion_time);
                    dequeue(ready_queue);
                }
                else{
                    log_process_event(current_ready, "Moved to WAIT queue for I/O");
                    enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst, current_ready->pcb.arrival_time, time_quantum);
                    
                    current_ready->pcb.completion_time = time_count;
                    wait_time += (current_ready->pcb.completion_time);
                    dequeue(ready_queue);
                }
            }
            else{
                log_process_event(current_ready, "Running");    
                current_ready->pcb.cpu_burst--; 
            }        
        }        
    }
    if(!isEmpty(wait_queue)){
        Node *current_wait = wait_queue->head;
        if(current_wait != NULL){
            if(current_wait->pcb.io_burst <= 0){
                if(current_wait->pcb.cpu_burst <= 0){
                    update_process_state(current_wait, TERMINATED);
                    log_process_event(current_wait, "Completed");
                    turnaround_time += time_count;
                    dequeue(wait_queue);
                }
            }   
            else{
                log_process_event(current_wait, "Running");    
                current_wait->pcb.io_burst--;
            }
        }   
    }
}

void terminate_scheduler(){
    removeQueue(ready_queue);
    removeQueue(wait_queue);
}