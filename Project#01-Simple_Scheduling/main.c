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

    if(signo == SIGALRM){
        if(!isEmpty(ready_queue)) {
            Node *current_ready = ready_queue->head;
            if(current_ready != NULL){
                if(current_ready->pcb.remaining_time == 0){ // 전환 필요 -> 부모 프로세스가 전환해줘야함 (요청 필요)
                    if(current_ready->pcb.cpu_burst <= 0){
                        if(current_ready->pcb.io_burst <= 0){ // 프로세스 종료
                            update_process_state(current_ready, TERMINATED);
                            log_process_event(current_ready, "Completed");
                            dequeue(ready_queue);
                        }
                        else{ // wait queue로 이동
                            log_process_event(current_ready, "Moved to WAIT queue for I/O");
                            enqueue(wait_queue, current_ready->pcb.pid, 0, current_ready->pcb.io_burst, TIME_QUANTUM);
                            dequeue(ready_queue);
                        }
                    }
                    else{ // 다시 레디 큐로 돌아감
                        log_process_event(current_ready, "Re-enqueued to Ready queue");
                        enqueue(ready_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, TIME_QUANTUM);
                        dequeue(ready_queue);
                    }
                }
                else if(current_ready->pcb.cpu_burst <= 0){
                    if(current_ready->pcb.io_burst <= 0){
                        update_process_state(current_ready, TERMINATED);
                        log_process_event(current_ready, "Completed");
                        dequeue(ready_queue);
                    }
                    else{
                        log_process_event(current_ready, "Re-enqueued from Wait queue to Ready queue");
                        enqueue(wait_queue, current_ready->pcb.pid, current_ready->pcb.cpu_burst, current_ready->pcb.io_burst, TIME_QUANTUM);
                        dequeue(ready_queue);
                    }
                }
                else{ // 단순 빼기
                    log_process_event(current_ready, "Running");    
                    current_ready->pcb.cpu_burst -= TIME_TICK;
                    current_ready->pcb.remaining_time -= TIME_TICK;  
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
                            dequeue(wait_queue);
                        }
                        else{ // ready queue로 이동
                            log_process_event(current_wait, "Moved to Ready queue");
                            enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, 0, TIME_QUANTUM);
                            dequeue(wait_queue);
                        }
                    }
                    else{ // 다시 wait 큐로 돌아감
                        log_process_event(current_wait, "Re-enqueued to Wait queue");
                        enqueue(wait_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, current_wait->pcb.io_burst, TIME_QUANTUM);
                        dequeue(wait_queue);
                    }
                }
                else if(current_wait->pcb.io_burst <= 0){
                    if(current_wait->pcb.cpu_burst <= 0){
                        update_process_state(current_wait, TERMINATED);
                        log_process_event(current_wait, "Completed");
                        dequeue(wait_queue);
                    }
                    else{
                        log_process_event(current_wait, "Re-enqueued from Wait queue to Ready queue");
                        enqueue(ready_queue, current_wait->pcb.pid, current_wait->pcb.cpu_burst, current_wait->pcb.io_burst, TIME_QUANTUM);
                        dequeue(wait_queue);
                    }
                }
                else{ // 단순 빼기
                    log_process_event(current_wait, "Running");    
                    current_wait->pcb.io_burst -= TIME_TICK;
                    current_wait->pcb.remaining_time -= TIME_TICK;
                }
            }
        }
        log_queue_state("READY Queue", ready_queue);
        log_queue_state("WAIT Queue", wait_queue);
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){ // 일단 보류  
        fprintf(stderr, "Usage: %s <time_tick> <burst_limit>.\n Ex) %s 10 60", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    int max_limit = atoi(argv[1]);

    struct timeval start, end;

    initialize_log("schedule_dump.txt");
    initialize_timer(scheduler_handler);
    initialize_scheduler();

    ready_queue = createQueue();
    wait_queue = createQueue();
    
    srand(time(NULL));


    gettimeofday(&start, NULL);

    for(int i = 0; i < MAX_PROCESSES; i++){
        int cpu_burst = rand() % max_limit + 1;
        int io_burst = rand() % max_limit + 1;

        Process *new_process = create_process(i, 0, cpu_burst, io_burst, TIME_QUANTUM);
        enqueue(ready_queue, new_process->pid, new_process->cpu_burst, new_process->io_burst, TIME_QUANTUM);
        log_process_event(new_process, "Created");

        printf("Created Process: PID = %d, CPU Burst = %d, IO Burst = %d\n",
               new_process->pid, new_process->cpu_burst, new_process->io_burst);
    }
    printf("\n");

    start_timer();

    while(!isEmpty(ready_queue) || !isEmpty(wait_queue)){
        pause();
    }

    gettimeofday(&end, NULL);

    double elapsed = end.tv_sec - start.tv_sec;

    close_log();
    stop_timer();
    terminate_scheduler();

    printf("All processes completed. Exiting.\n");

    printf("=====================================\n");
    printf("Total Executing Time: %.3f\n", elapsed);
    printf("=====================================\n");

    return 0;
}
