#pragma once

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "common.h"

typedef struct{
    long mtype;

    int pid;
    int cpu_burst;
    int io_burst;
    int state;
}Message;

void initialize_ipc();

int send_msg(int msgid, int pid, int cpu_burst, int io_burst, int state);

int recv_msg(int msgid, Message *msg, long msg_type);

void* attach_shared_memory(int shmid);

void cleanup_ipc();