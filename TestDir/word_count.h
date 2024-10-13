#ifndef WORD_COUNT_H
#define WORD_COUNT_H

#include "bits/stdc++.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <cstring>

#define MAX_STRING_LENGTH 30 // 문자열의 최대 길이
#define ASCII_SIZE 256 // 아스키 문자 테이블 크기

struct SharedObject {
    std::ifstream rfile;
    int linenum = 0;
    std::string line;
    int stat[MAX_STRING_LENGTH];  // 문자열 길이에 대한 빈도수 저장
    int stat2[ASCII_SIZE];        // 아스키 문자 빈도수 저장
    std::mutex lock;
    bool full = false;
    bool finished = false;
    std::condition_variable cond_var;

    SharedObject() {
        std::memset(stat, 0, sizeof(stat));    // stat 초기화
        std::memset(stat2, 0, sizeof(stat2));  // stat2 초기화
    }
};

void process_line(SharedObject& so, const std::string& line) {
    size_t length = line.length();

    // 문자열 길이 통계
    if (length >= MAX_STRING_LENGTH) {
        length = MAX_STRING_LENGTH;
    }
    so.stat[length - 1]++;

    // 아스키 문자 빈도수 통계
    for (char ch : line) {
        if (ch >= 1 && ch < ASCII_SIZE) {
            so.stat2[(int)ch]++;
        }
    }
}

void producer(std::shared_ptr<SharedObject> so, int *ret);
void consumer(std::shared_ptr<SharedObject> so, int *ret);

#endif
