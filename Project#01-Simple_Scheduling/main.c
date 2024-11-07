#include "common.h"
#include "scheduling.h"
#include "timer.h"
#include "ipc.h"
#include "queue.h"
#include "process.h"
#include "log.h"

Queue *ready_queue;
Queue *wait_queue;

int time_count = 0;

void scheduler_handler(int signo){
    time_count++;

    if(signo == SIGALRM && time_count == TIME_QUANTUM){ // 이때 전환이 일어나는거임
        if(!isEmpty(ready_queue)) {
            Node *current_ready = ready_queue->head;
        
            if(current_ready != NULL){
                log_process_event(current_ready, "Running");
                current_ready->pcb.cpu_burst -= TIME_TICK/10;
                
                if(current_ready->pcb.cpu_burst <= 0){ // cpu 끝났고
                    if(current_ready->pcb.io_burst > 0){ // io는 안끝나면
                        printf("RR PID DEBUG: %d\n", current_ready->pcb.pid);
                        
                        log_process_event(current_ready, "First Moved to WAIT queue for I/O"); // wait queue로 이동
                        enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst);
                        dequeue(ready_queue);
                        
                    }
                    else{ // 둘다 끝난거면 끝
                        update_process_state(current_ready, TERMINATED);
                        log_process_event(current_ready, "Completed123");
                        
                        printf("1. Finished Process ID: %d\n", current_ready->pcb.pid);
                        dequeue(ready_queue);
                    }
                }
                else{ // cpu > 0
                    if(current_ready->pcb.io_burst <= 0){ // io <= 0
                        printf("RE ENQUEUED RR PID DEBUG: %d\n", current_ready->pcb.pid);

                        log_process_event(current_ready, "First Re-enqueued to READY queue");
                        enqueue(ready_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, 0);
                        dequeue(wait_queue);

                    }
                    else{
                        log_process_event(current_ready, "Second Moved to WAIT queue for I/O");
                        enqueue(wait_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst);
                        dequeue(ready_queue);
                    }
                }
            }
        }

        if(!isEmpty(wait_queue)){
            Node *current_wait = wait_queue->head; // 이동하고 줘서 그런가? 순서 문제?

            if(current_wait != NULL){
                    if(current_wait->pcb.io_burst <= 0){
                        if(current_wait->pcb.cpu_burst > 0){
                            log_process_event(current_wait, "First Moved to READY queue after I/O");
                            enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0);
                            dequeue(wait_queue);
                            
                        }
                        else{
                            update_process_state(current_wait, TERMINATED);
                            log_process_event(current_wait, "Completed456"); 
                            printf("3. Finished Process ID: %d\n", current_wait->pcb.pid);   
                            printf("Freeing complete I/O process with PID: %d\n", current_wait->pcb.pid);
                            dequeue(ready_queue);
                        }
                    }
                    else{ // io가 0보다 큼
                        if(current_wait->pcb.cpu_burst > 0){
                            log_process_event(current_wait, "First Moved to READY queue after I/O");
                            enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0);
                            dequeue(wait_queue);
                        }
                        else{
                            log_process_event(current_wait, "First Re-Moved to WAIT queue after I/O");
                            enqueue(wait_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0);
                            dequeue(wait_queue);
                        }
                    }
                }
            }   
        
            time_count = 0;
        }
    

    else { // 단순 빼기, 단, 0보다 작아지면 끝내거나 전환해줌
        if (!isEmpty(wait_queue)) {  // wait_queue가 비어 있는지 확인
            Node *current_wait = wait_queue->head;

            if (current_wait != NULL) {
                if(current_wait->pcb.io_burst <= 0){
                    if(current_wait->pcb.cpu_burst <= 0){
                        update_process_state(current_wait, TERMINATED);
                        log_process_event(&current_wait->pcb, "Completed789");
                        printf("4. Finished Process ID: %d\n", current_wait->pcb.pid);

                        dequeue(wait_queue);
                    }
                    else{
                        log_process_event(&current_wait->pcb, "Second Moved to Ready queue for I/O");
                        enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0);
                        dequeue(wait_queue);
                    }
                }
                else{
                    current_wait->pcb.io_burst -=TIME_TICK/10;
                }     
            }
        }
    }
        if (!isEmpty(ready_queue)) {  // ready_queue가 비어 있는지 확인
            Node *current_ready = ready_queue->head;

            if (current_ready != NULL) {
                if(current_ready->pcb.cpu_burst <= 0){
                    if(current_ready->pcb.io_burst <= 0){ // 여기서 문제
                        update_process_state(current_ready, TERMINATED);
                        log_process_event(&current_ready->pcb, "Completed101112");
                        puts("HELLO!!!!!!!!!!\n");
                        printf("Finished Process ID: %d\n", current_ready->pcb.pid);
                        dequeue(ready_queue);
                        // free(current_ready);
                    }
                    else{
                        puts("DEBUG NO TIMEQUANTUM CURRENT READY ERROR\n");
                        log_process_event(&current_ready->pcb, "Third Moved to Wait queue for I/O");
                        enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst);
                        dequeue(ready_queue);
                    }
                }
                else{
                    current_ready->pcb.cpu_burst -= TIME_TICK/10;
                }
            }
        }
    

    log_queue_state("READY Queue", ready_queue); // 여기?
    log_queue_state("WAIT Queue", wait_queue);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s <burst_limit>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int max_limit = atoi(argv[1]);

    initialize_log("schedule_dump.txt");
    initialize_timer(scheduler_handler);
    initialize_scheduler();

    

    ready_queue = createQueue();
    wait_queue = createQueue();
    
    for(int i = 0; i < MAX_PROCESSES; i++){
        int cpu_burst = rand() % max_limit + 1;
        int io_burst = rand() % max_limit + 1;

        Process *new_process = create_process(i, 0, cpu_burst, io_burst);
        enqueue(ready_queue, new_process->pid, new_process->cpu_burst, new_process->io_burst); // 여기까지 io_burst 잘 전달되고 있음
        log_process_event(new_process, "Created");

        printf("Created Process: PID = %d, CPU Burst = %d, IO Burst = %d\n",
               new_process->pid, new_process->cpu_burst, new_process->io_burst);
    }

    start_timer();

    while(!isEmpty(ready_queue) || !isEmpty(wait_queue)){
        pause();
    }

    puts("LETS FINISH\n");
    close_log();
    stop_timer();
    terminate_scheduler();

    printf("All processes completed. Exiting.\n");
    return 0;
}
