#include "word_count.h"

void producer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;
    string line;

    while (true) {
        unique_lock<mutex> ulock(so->lock);
        so->cond_var.wait(ulock, [so]() { return !so->full; });

        if (!getline(so->rfile, line)) {
            // 파일 끝에 도달했을 때
            so->finished = true;
            so->cond_var.notify_all();  // 마지막 알림 전송
            break;
        }

        // 파일에서 읽은 줄 저장
        so->linenum = i;
        so->line = line;
        i++;
        so->full = true;

        // 소비자에게 알림
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

        // 파일의 끝에 도달한 경우
        if (so->finished) {
            break;  // 더 이상 처리할 데이터가 없으면 종료
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