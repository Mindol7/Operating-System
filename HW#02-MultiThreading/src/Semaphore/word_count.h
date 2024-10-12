#pragma once

#include "bits/stdc++.h"

#include <semaphore>

#include <stdlib.h>
#include <unistd.h>

#define MAX_STRING_LENGTH 30 // 문자열의 최대 길이
#define ASCII_SIZE	256 // 아스키 문자 테이블 크기

using namespace std;

struct SharedObject {
    ifstream rfile; // 입력 파일을 읽는 역할을 함
    int linenum = 0; // 현재 읽고 있는 줄 번호 저장
    string line; // 현재 읽은 한줄을 저장

    /* 동기화 문제를 해결하기 위한 변수 추가 */
    bool finished = false;
    int stat[MAX_STRING_LENGTH];
    int stat2[ASCII_SIZE];
    mutex mtx;

    /* 세마포어 */
    counting_semaphore<1> empty{1};
    counting_semaphore<1> full{0};
    

    SharedObject(){
        memset(stat, 0, sizeof(stat));
        memset(stat2, 0, sizeof(stat2));
    }
};

void process_line(SharedObject& so, const string& line);
void print_statistics(SharedObject& so);
void producer(shared_ptr<SharedObject> so, int *ret);
void consumer(shared_ptr<SharedObject> so, int *ret);