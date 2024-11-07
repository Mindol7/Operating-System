#include "process.h"

Process* create_process(int pid, int arrival_time, int cpu_burst, int io_burst){
    Process *new_process = (Process*)malloc(sizeof(Process));
    if(new_process == NULL){
        perror("Failed to create process");
        exit(EXIT_FAILURE);
    }

    new_process->pid = pid;
    new_process->arrival_time = arrival_time;
    new_process->cpu_burst = cpu_burst;
    new_process->io_burst = io_burst;
    new_process->state = NEW;

    return new_process;
}

void update_process_state(Process *process, ProcessState new_state){
    if(process->state != new_state){
        process->state = new_state;
    }
}

void terminate_process(Process *process){
    if(process){
        printf("Terminating process %d\n", process->pid);
        free(process);
    }
}

void print_process_info(const Process *process){
    printf("Process ID: %d\n", process->pid);
    printf("Arrival Time: %d\n", process->arrival_time);
    printf("CPU Burst: %d\n", process->cpu_burst);
    printf("I/O Burst: %d\n", process->io_burst);
    printf("State: %d\n", process->state);
}
