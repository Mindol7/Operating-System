#include "word_count.h"

void producer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;
    string line;

    while (true) {
        so->empty.acquire();

        unique_lock<mutex> lock(so->mtx);

        if (!getline(so->rfile, line)) {
            // 파일 끝에 도달했을 때
            so->finished = true;
            so->full.release();  // 마지막 알림 전송
            break;
        }

        // 파일에서 읽은 줄 저장
        so->linenum = i;
        so->line = line;

        i++;
        so->full.release();  // 소비자에게 알림
    }
    *ret = i;
    cout<<"Prod_"<<this_thread::get_id()<<": "<<i<<"lines\n";
}

void consumer(shared_ptr<SharedObject> so, int *ret) {
    int i = 0;

    while (true) {
        so->full.acquire();  // 데이터가 있을 때만 진입

        unique_lock<mutex> lock(so->mtx);

        // 모든 프로듀서가 종료하고 데이터가 없을 때 종료
        if (so->finished) {
            so->empty.release();
            so->full.release();
            break;
        }

        process_line(*so, so->line);  // 줄을 처리
        cout<<"Cons_"<<this_thread::get_id()<<":["<<i<<": "<<so->linenum<<"] "<<so->line<<endl;
        i++;

        so->empty.release();  // 프로듀서가 데이터를 더 넣을 수 있도록 함
    }

    *ret = i;
    cout<<"Cons: "<<i<<" lines\n";
}
