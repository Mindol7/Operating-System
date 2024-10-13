/*
* Author: Minhyuk Cho
* Date: 2024-10-12
* Description: Semaphore Ver. minhyuk_prod_cons.cpp
*/

#include "word_count.h"

void producer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;
    char buffer[BUFFER_SIZE];
    string line;

    while (true) {
        so->empty.acquire();

        unique_lock<mutex> lock(so->mtx);

        if (!so->rfile.read(buffer, BUFFER_SIZE)) {
            if(so->rfile.gcount() == 0){    
                so->finished = true;
                so->full.release(); 
                break;
            }
        }

        stringstream ss(string(buffer, so->rfile.gcount()));

        while(getline(ss, line)){
            so->linenum = i;
            so->line[so->producer_idx] = line;

            so->producer_idx = (so->producer_idx + 1) % BUFFER_SIZE;

            i++;
            so->full.release();
        }
    }

    *ret = i;
    cout<<"Prod_"<<this_thread::get_id()<<": "<<i<<"lines\n";
}

void consumer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;

    while (true) {
        so->full.acquire();

        unique_lock<mutex> lock(so->mtx);

        if (so->finished && so->producer_idx == so->consumer_idx) {
            // so->empty.release();
            // so->full.release();
            break;
        }

        process_line(*so, so->line[so->consumer_idx]);

        so->consumer_idx  = (so->consumer_idx + 1) % BUFFER_SIZE;

        cout<<"Cons_"<<this_thread::get_id()<<":["<<i<<": "<<so->linenum<<"] "<<so->line<<endl;
        i++;

        so->empty.release();
    }

    *ret = i;
    cout<<"Cons: "<<i<<" lines\n";
}
