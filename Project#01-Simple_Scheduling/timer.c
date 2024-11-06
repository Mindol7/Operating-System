#include "timer.h"

static struct itimerval timer;

void initialize_timer(void (*handler)(int)){
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGALRM, &sa, NULL) == -1){
        perror("Error setting signal handler");
        exit(EXIT_FAILURE);
    }

    timer.it_value.tv_sec = TIMER_INTERVAL / 1000;
    timer.it_value.tv_usec = (TIMER_INTERVAL % 1000) * 1000;
    timer.it_interval.tv_sec = TIMER_INTERVAL / 1000;
    timer.it_interval.tv_usec = (TIMER_INTERVAL % 1000) * 1000;
}

void start_timer(){
    if(setitimer(ITIMER_REAL, &timer, NULL) == -1){
        perror("Error starting timer");
        exit(EXIT_FAILURE);
    }
}

void stop_timer(){
    struct itimerval stop = {0};
    if(setitimer(ITIMER_REAL, &stop, NULL) == -1){
        perror("Error stopping timer");
        exit(EXIT_FAILURE);
    }
}