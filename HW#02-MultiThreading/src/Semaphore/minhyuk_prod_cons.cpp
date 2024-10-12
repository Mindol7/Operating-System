/*
* Author: Minhyuk Cho
* Date: 2024-10-12
* Description: Semaphore Ver. minhyuk_prod_cons.cpp
*/

#include "word_count.h"

void producer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;
    string line;

    while (true) {
        so->empty.acquire();

        unique_lock<mutex> lock(so->mtx);

        if (!getline(so->rfile, line)) {
            so->finished = true;
            so->full.release(); 
            break;
        }

        so->linenum = i;
        so->line = line;

        i++;
        so->full.release();
    }
    *ret = i;
    cout<<"Prod_"<<this_thread::get_id()<<": "<<i<<"lines\n";
}

void consumer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;

    while (true) {
        so->full.acquire();

        unique_lock<mutex> lock(so->mtx);

        if (so->finished) {
            so->empty.release();
            so->full.release();
            break;
        }

        process_line(*so, so->line);
        cout<<"Cons_"<<this_thread::get_id()<<":["<<i<<": "<<so->linenum<<"] "<<so->line<<endl;
        i++;

        so->empty.release();
    }

    *ret = i;
    cout<<"Cons: "<<i<<" lines\n";
}
