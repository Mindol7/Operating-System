#include "ipc.h"

static int msgid;
static int shmid;
static int semid;

void initialize_ipc(){
    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if(msgid == -1){
        perror("Failed to create message queue");
        exit(EXIT_FAILURE);
    }

    shmid = shmget(IPC_PRIVATE, sizeof(Message) * MAX_PROCESSES, IPC_CREAT | 0666);
    if(shmid == -1){
        perror("Failed to create shared memory");
        exit(EXIT_FAILURE);
    }

    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if(semid == -1){
        perror("Failed to create semaphore");
        exit(EXIT_FAILURE);
    }

    semctl(semid, 0, SETVAL, 1);
}

int send_msg(int msgid, int pid, int cpu_burst, int io_burst, int state){
    Message msg;
    msg.mtype = pid;
    msg.pid = pid;
    msg.cpu_burst = cpu_burst;
    msg.io_burst = io_burst;
    msg.state = state;

    if(msgsnd(msgid, &msg, sizeof(Message) - sizeof(long), 0) == -1){
        perror("Failed to send message");
        return -1;
    }

    return 0;
}

int recv_msg(int msgid, Message *msg, long msg_type){
    if(msgrcv(msgid, msg, sizeof(Message) - sizeof(long), msg_type, 0) == -1){
        perror("Failed to receive mssage");
        return -1;
    }

    return 0;
}

void* attach_shared_memory(int shmid){
    void *shm_ptr = shmat(shmid, NULL, 0);
    if(shm_ptr == (void*)-1){
        perror("Failed to attach shared memory");
        exit(EXIT_FAILURE);
    }

    return shm_ptr;
}

void cleanup_ipc(){
    if(msgctl(msgid, IPC_RMID, NULL) == -1) perror("Failed to remove message queue");
    if(shmctl(shmid, IPC_RMID, NULL) == -1) perror("Failed to remove shared queue");
    if(semctl(semid, IPC_RMID, NULL) == -1) perror("Failed to remove semaphore");
}