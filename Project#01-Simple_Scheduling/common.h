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

#define TIME_QUANTUM 1000
#define MAX_PROCESSES 10
#define TIME_TICK 1000
//#define LOG_FILE "schedule_dump.txt"

extern int counter;
extern int proc_count;
extern int burst_limit;

extern int wait_time;
extern int turnaround_time;
extern int run_time;