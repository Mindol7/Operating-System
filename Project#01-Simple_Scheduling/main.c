#include "common.h"
#include "scheduling.h"
#include "timer.h"
#include "queue.h"
#include "process.h"
#include "log.h"

Queue *ready_queue;
Queue *wait_queue;

Process **parent;

int time_tick;
int time_quantum;

int time_count;

double wait_time;
double turnaround_time;

void scheduler_handler(int signo){
    if(signo == SIGALRM){
        time_count++;
        schedule();
        
        log_queue_state("READY Queue", ready_queue);
        log_queue_state("WAIT Queue", wait_queue);
    }
}

int main(int argc, char *argv[]){
    if(argc < 4){  
        fprintf(stderr, "Usage: %s <TIME_TICK> <TIME_QUANTUM> <BURST_LIMIT>.\n Ex) %s 1 5 10\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    time_tick = atoi(argv[1]);
    time_quantum = atoi(argv[2]);
    int max_limit = atoi(argv[3]);

    time_count = 0;
    wait_time = 0.0;
    turnaround_time = 0.0;
    struct timeval start, end;

    initialize_log("schedule_dump.txt");
    initialize_timer(scheduler_handler);
    initialize_scheduler();

    ready_queue = createQueue();
    wait_queue = createQueue();
    
    srand(time(NULL));

    gettimeofday(&start, NULL);

    parent = (Process*)malloc(sizeof(Process*)*MAX_PROCESSES);

    if(parent == NULL){
        perror("Failed to Allocation Memory.\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < MAX_PROCESSES; i++){
        int cpu_burst = rand() % max_limit + 1;
        int io_burst = rand() % max_limit + 1;

        // Process *new_process = create_process(i, 0, cpu_burst, io_burst, time_quantum);
        parent[i] = create_process(i, 0, cpu_burst, io_burst, time_quantum);
        enqueue(ready_queue, parent[i]->pid, parent[i]->cpu_burst, parent[i]->io_burst, time_quantum);
        log_process_event(parent[i], "Created");

        printf("Created Process: PID = %d, CPU Burst = %d, IO Burst = %d\n",
               parent[i]->pid, parent[i]->cpu_burst, parent[i]->io_burst);
    }
    printf("\n");

    start_timer();

    while(!isEmpty(ready_queue) || !isEmpty(wait_queue)){
        pause();
    }

    gettimeofday(&end, NULL);

    double elapsed = end.tv_sec - start.tv_sec;

    wait_time = turnaround_time - elapsed;

    close_log();
    stop_timer();
    terminate_scheduler();

    printf("All processes completed. Exiting.\n");

    printf("=====================================\n");
    printf("Total Run Time: %.3f (sec)\n", elapsed);
    printf("Avg. Wait Time: %.3f (sec) \n", wait_time / MAX_PROCESSES);
    printf("Avg. Turnaround Time: %.3f (sec)\n", turnaround_time / MAX_PROCESSES);
    printf("=====================================\n");

    return 0;
}
