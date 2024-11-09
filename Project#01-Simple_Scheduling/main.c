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
double total_wait_time;

int schedule_policy;

void scheduler_handler(int signo){
    if(signo == SIGALRM){
        switch(schedule_policy){
            case 1:
                FCFS_schedule();
                break;
            case 2:
                RR_schedule();
                break;
            case 3:
                SJF_schedule();
                break;
            default:
                perror("No Scheduling");
                exit(EXIT_FAILURE);
        }
        
        log_queue_state("READY Queue", ready_queue);
        log_queue_state("WAIT Queue", wait_queue);
        time_count++;
    }
}

int main(int argc, char *argv[]){
    if(argc < 3){  
        fprintf(stderr, "Usage-1: %s <SCHEDULING POLICY> <TIME TICK> <BURST_LIMIT>.\n", argv[0]);
        fprintf(stderr, "Usage-2: '1' is FCFS Scheduling, '2' is Round Robin Scheduling, '3' is SJF Scheduling\n");
        exit(EXIT_FAILURE);
    }

    schedule_policy = atoi(argv[1]);
    
    time_tick = atoi(argv[2]);
    int max_limit = atoi(argv[3]);

    time_quantum = 0;

    if(schedule_policy == 2){    
        printf("========= Round Robin Scheduling =========\n");
        printf("Input Prgram TIME QUANTUM: ");
        scanf("%d", &time_quantum);
        printf("==========================================\n");
    }


    time_count = 0;

    wait_time = 0.0;
    turnaround_time = 0.0;
    total_wait_time = 0.0;
    struct timeval start, end;

    switch(schedule_policy){
        case 1:    
            initialize_log("schedule_FCFS_dump.txt");
            break;
        case 2:        
            initialize_log("schedule_RR_dump.txt");
            break;
        case 3:
            initialize_log("schedule_SJF_dump.txt");
            break;
        default:
            perror("No dump\n");
            exit(EXIT_FAILURE);
    }

    initialize_timer(scheduler_handler);
    initialize_scheduler();

    ready_queue = createQueue();
    wait_queue = createQueue();
    
    srand(time(NULL));

    gettimeofday(&start, NULL);

    parent = (Process*)malloc(sizeof(Process*)*MAX_PROCESSES);

    for(int i = 0; i < MAX_PROCESSES; i++){
        int cpu_burst = rand() % max_limit + 1;
        int io_burst = rand() % max_limit + 1;

        parent[i] = create_process(i, cpu_burst, io_burst, time_quantum);
        enqueue(ready_queue, parent[i]->pid, parent[i]->cpu_burst, parent[i]->io_burst, 0, time_quantum);
        log_process_event(parent[i], "Created");
        printf("Created Process: PID = %d, CPU Burst = %d, IO Burst = %d\n",
               parent[i]->pid, parent[i]->cpu_burst, parent[i]->io_burst);
    }
    printf("\n");

    if(schedule_policy == 3){
        sort_queue(ready_queue);
    }

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
    printf("Total Run Time: %.3f (sec)\n", elapsed);
    printf("Avg. Wait Time: %.3f (sec) \n", total_wait_time / MAX_PROCESSES);
    printf("Avg. Turnaround Time: %.3f (sec)\n", turnaround_time / MAX_PROCESSES);
    printf("=====================================\n");

    return 0;
}
