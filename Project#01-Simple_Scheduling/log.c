#include "log.h"

static FILE *log_file = NULL;

void initialize_log(const char *filename){
    log_file = fopen(filename, "w");

    if(log_file == NULL){
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }
    fprintf(log_file, "-----Scheduling Log-----\n");
    fprintf(log_file, "Log initialized at: %s\n\n", get_timestamp());
}

const char* get_timestamp(){
    static char buffer[20];
    time_t now = time(NULL);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));

    return buffer;
}

void log_process_event(const Process *process, const char *event){
    if(log_file){
        fprintf(log_file, "[%s] Process %d - %s\n", get_timestamp(), process->pid, event);
    }
}

void log_queue_state(const char *queue_name, const Queue *queue){
    if(log_file){
        fprintf(log_file, "[%s] Queue State - %s\n", get_timestamp(), queue_name);

        Node *current = queue->head;

        while(current != NULL){
            fprintf(log_file, "Process ID: %d, CPU Burst: %d, IO Burst: %d, Remaining: %d\n",
                    current->pcb.pid, current->pcb.cpu_burst, current->pcb.io_burst, current->pcb.remaining_time);
            current = current->next;
        }
        fprintf(log_file, "\n");
    }
}

void close_log(){
    if(log_file){
        fprintf(log_file, "----- End of Log -----\n");
        fclose(log_file);
        log_file = NULL;
    }
}