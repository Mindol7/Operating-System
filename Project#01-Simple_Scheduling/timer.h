#pragma once

#include "common.h"

/* 타이머 간격 */
#define TIMER_INTERVAL 1000

/* 타이머 초기화 함수 */
void initialize_timer(void (*handler)(int));

/* 타이머 시작 함수 */
void start_timer();

/* 타이머 정지 함수 */
void stop_timer();