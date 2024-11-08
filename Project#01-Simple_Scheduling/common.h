#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#define TIME_QUANTUM 5
#define MAX_PROCESSES 10
#define TIME_TICK 1

extern int counter;
extern int proc_count;
extern int burst_limit;

extern int time_count;

extern int time_tick;
extern int time_quantum;

extern double wait_time;
extern double turnaround_time;
