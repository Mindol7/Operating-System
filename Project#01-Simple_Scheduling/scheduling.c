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

            if(current_ready->pcb.remaining_time == 0){ // 타임퀀텀 다씀, 전환 필요 -> 부모 프로세스가 전환해줘야함 (요청 필요)
                if(current_ready->pcb.cpu_burst <= 0){
                    if(current_ready->pcb.io_burst <= 0){ // 프로세스 종료
                        update_process_state(current_ready, TERMINATED);
                        log_process_event(current_ready, "Completed");
                        
                        current_ready->pcb.completion_time = time_count;
                        turnaround_time += current_ready->pcb.completion_time - current_ready->pcb.arrival_time;
                        wait_time += current_ready->pcb.total_wait_time;

                        dequeue(ready_queue);
                    }
                    else{ // wait queue로 이동
                        log_process_event(current_ready, "Moved to WAIT queue for I/O");
                        enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst, time_quantum);
                        
                        current_ready->pcb.total_wait_time += (time_count - current_ready->pcb.start_time);
                        
                        dequeue(ready_queue);
                    }
                }
                else{ // 다시 레디 큐로 돌아감
                    log_process_event(current_ready, "Re-enqueued to Ready queue");
                    enqueue(ready_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, time_quantum);
    
                    current_ready->pcb.total_wait_time += (time_count - current_ready->pcb.start_time);
                    dequeue(ready_queue);
                }
            }
            else if(current_ready->pcb.cpu_burst <= 0){ // cpu 다함
                if(current_ready->pcb.io_burst <= 0){
                    update_process_state(current_ready, TERMINATED);
                    log_process_event(current_ready, "Completed");
                    
                    current_ready->pcb.completion_time = time_count;
                    turnaround_time += current_ready->pcb.completion_time - current_ready->pcb.arrival_time;
                    wait_time += current_ready->pcb.total_wait_time;

                    dequeue(ready_queue);
                }
                else{ // cpu < 0, io > 0
                    log_process_event(current_ready, "Re-enqueued to Wait queue");
                    enqueue(wait_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, time_quantum);
                    
                    current_ready->pcb.total_wait_time += (time_count - current_ready->pcb.start_time);
                    dequeue(ready_queue);
                }
            }
            else{ // 단순 빼기
                log_process_event(current_ready, "Running");    
                current_ready->pcb.cpu_burst -= time_tick;
                current_ready->pcb.remaining_time -= time_tick;  
            }
        }
    }

    if(!isEmpty(wait_queue)){// burst 다되면 전환 필요
        Node *current_wait = wait_queue->head;
        if(current_wait != NULL){
            if(current_wait->pcb.remaining_time == 0){ // 전환 필요
                if(current_wait->pcb.io_burst <= 0){
                    if(current_wait->pcb.cpu_burst <= 0){ // 프로세스 종료
                        update_process_state(current_wait, TERMINATED);
                        log_process_event(current_wait, "Completed");

                        
                        current_wait->pcb.completion_time = time_count;
                        turnaround_time += current_wait->pcb.completion_time - current_wait->pcb.arrival_time;
                        wait_time += current_wait->pcb.total_wait_time;

                        dequeue(wait_queue);
                    }
                    else{ // ready queue로 이동
                        log_process_event(current_wait, "Moved to Ready queue");
                        enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0, time_quantum);
                        dequeue(wait_queue);
                    }
                }
                else{ // 다시 wait 큐로 돌아감
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
                    wait_time += current_wait->pcb.total_wait_time;


                    dequeue(wait_queue);
                }
                else{
                    log_process_event(current_wait, "Re-enqueued from Wait queue to Ready queue");
                    enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0, time_quantum);
                    dequeue(wait_queue);
                }
            }
            else{ // 단순 빼기
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