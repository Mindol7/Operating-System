/*
* Author: Minhyuk Cho
* Date: 2024-10-12
* Description: Semaphore Ver. word_count.h
*/

#pragma once

#include "bits/stdc++.h"

#include <semaphore>

#include <stdlib.h>
#include <unistd.h>

#define MAX_STRING_LENGTH 30
#define ASCII_SIZE	256

using namespace std;

struct SharedObject {
    ifstream rfile;
    int linenum = 0;
    string line;

    bool finished = false;
    int stat[MAX_STRING_LENGTH];
    int stat2[ASCII_SIZE];
    mutex mtx;

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