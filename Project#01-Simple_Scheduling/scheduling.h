#pragma once

#include "process.h"

/* 스케줄링 초기화 함수 */
void initialize_scheduler(); // Ver.0 -> Round Robin

/* 다음 프로세스 스케줄링 함수 */
Process* schedule_next();

/* 스케줄링 상태 업데이트 */
void update_scheduler(Process *process);

/* 스케줄링 종료 함수 */
void terminate_scheduler();