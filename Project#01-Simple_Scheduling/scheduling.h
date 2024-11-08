#pragma once

#include "process.h"
#include "queue.h"

/* 스케줄링 초기화 함수 */
void initialize_scheduler(); 

void schedule();

/* 스케줄링 종료 함수 */
void terminate_scheduler();
