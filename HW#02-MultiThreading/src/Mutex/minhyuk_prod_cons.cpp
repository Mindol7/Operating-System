#include "word_count.h"

void producer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;
    string line;

    while (true) {
        unique_lock<mutex> ulock(so->lock);
        so->cond_var.wait(ulock, [so]() { return !so->full; });

        if (!getline(so->rfile, line)) {
            so->finished = true;
            so->cond_var.notify_all();
            break;
        }

        so->linenum = i;
        so->line = line;
        i++;
        so->full = true;

        so->cond_var.notify_all();
    }
    *ret = i;
    cout << "Prod_" << this_thread::get_id() << ": " << i << " lines\n";

}

void consumer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;

    while (true) {
        unique_lock<mutex> ulock(so->lock);
        so->cond_var.wait(ulock, [so]() { return so->full || so->finished; });

        if (so->finished) {
            break;
        }

        if(!so->full){
            so->cond_var.notify_all();
            break;
        }

        process_line(*so, so->line);
        
        cout << "Cons_" << this_thread::get_id() << ": [" << i << ":" << so->linenum << "] " << so->line << endl;
        i++;
        so->full = false;

        so->cond_var.notify_all();
    }

    *ret = i;
    cout << "Cons: " << i << " lines\n";
}