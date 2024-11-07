#pragma once

#include "common.h"

typedef enum{
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} ProcessState;

typedef struct{
    int pid;
    int arrival_time;
    int cpu_burst;
    int io_burst;
    int remaining_time;
    ProcessState state;
} Process;

Process* create_process(int pid, int arrival_time, int cpu_burst, int io_burst);
void update_process_state(Process *process, ProcessState new_state);
void terminate_process(Process *process);
void print_process_info(const Process *process);
