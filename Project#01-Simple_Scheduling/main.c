#include "common.h"
#include "scheduling.h"
#include "timer.h"
#include "ipc.h"
#include "queue.h"
#include "process.h"
#include "log.h"

Queue *ready_queue;

void scheduler_handler(int signo){
    if(signo == SIGALRM){
        Process *next_process = schedule_next();
    
        if(next_process != NULL){
            log_process_event(next_process, "Running");
            next_process->remaining_time--;

            if(next_process->remaining_time == 0){
                update_process_state(next_process, TERMINATED);
                log_process_event(next_process, "Completed");
            }
            else{
                update_process_state(next_process, READY);
            }
        }
    }

    log_queue_state("ready Queue", ready_queue);

}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s <burst_limit>", argv[0]);
        exit(EXIT_FAILURE);
    }
    puts("HI\n");
    int max_limit = atoi(argv[1]);

    initialize_log("schedule_dump.txt");

    initialize_ipc();
    initialize_timer(scheduler_handler);
    initialize_scheduler();

    start_timer();

    Queue *ready_queue = createQueue();
    for(int i = 0; i < MAX_PROCESSES; i++){
        int cpu_burst = rand() % max_limit + 1;
        int io_burst = rand() % max_limit + 1;

        Process *new_process = create_process(i, 0, cpu_burst, io_burst);
        enqueue(ready_queue, new_process->pid, new_process->cpu_burst, new_process->io_burst);
        log_process_event(new_process, "Created");

        printf("Created Process: PID = %d, CPU Burst = %d, IO Burst = %d\n",
                new_process->pid, new_process->cpu_burst, new_process->io_burst);
    }

    while(!isEmpty(ready_queue)) pause();

    close_log();
    cleanup_ipc();
    stop_timer();
    terminate_scheduler();
    removeQueue(ready_queue);

    printf("All processes completed. Exiting.\n");

    return 0;
}