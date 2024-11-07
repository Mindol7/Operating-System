// #include "ipc.h"

// static int msgid;

// void initialize_ipc(){
//     msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
//     if(msgid == -1){
//         perror("Failed to create message queue");
//         exit(EXIT_FAILURE);
//     }
// }

// int send_msg(int msgid, int pid, int cpu_burst, int io_burst, int state){
//     Message msg;
//     msg.mtype = pid;
//     msg.pid = pid;
//     msg.cpu_burst = cpu_burst;
//     msg.io_burst = io_burst;
//     msg.state = state;

//     if(msgsnd(msgid, &msg, sizeof(Message) - sizeof(long), IPC_NOWAIT) == -1){
//         perror("Failed to send message");
//         return -1;
//     }

//     return 0;
// }

// int recv_msg(int msgid, Message *msg, long msg_type){
//     if(msgrcv(msgid, msg, sizeof(Message) - sizeof(long), msg_type, IPC_NOWAIT) == -1){
//         if (errno != ENOMSG){
//             perror("Failed to receive message");
//         }
//         return -1;
//     }

//     return 0;
// }

// void cleanup_ipc(){
//     if (msgctl(msgid, IPC_RMID, NULL) == -1) perror("Failed to remove message queue");
// }
