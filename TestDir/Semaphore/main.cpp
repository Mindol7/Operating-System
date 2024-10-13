#include "word_count.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        cerr << "usage: ./prod_cons <readfile> #Producer #Consumer\n";
        return 1;
    }

    int Nprod = (argc > 2) ? min(atoi(argv[2]), 100) : 1;
    int Ncons = (argc > 3) ? min(atoi(argv[3]), 100) : 1;

    auto share = make_shared<SharedObject>();
    share->rfile.open(argv[1]);
    
    if (!share->rfile.is_open()) {
        cerr << "Error opening file\n";
        return 1;
    }

    vector<thread> producers;
    vector<thread> consumers; 
    vector<int> prod_results(Nprod);
    vector<int> cons_results(Ncons);
    int i = 0;
    int sum;
    
    // 생산자 스레드 생성
    for (int i = 0; i < Nprod; ++i) {
        producers.emplace_back(producer, share, &prod_results[i]); // 스레드니까 함수를 호출할 스레드 생성하고 매개변수 같이 전달해줌
    }

    // 소비자 스레드 생성
    for (int i = 0; i < Ncons; ++i) {
        consumers.emplace_back(consumer, share, &cons_results[i]);
    }

    // 소비자 스레드 종료 대기
    for (auto& th : consumers) {
        if (th.joinable()) {
            th.join();
            cout<<"main: consumer_"<<i<<" joined with "<<cons_results[i]<<endl;
        }
        i++;
    }

    i = 0;

    // 생산자 스레드 종료 대기
    for (auto& th : producers) {
        if (th.joinable()) {
            th.join();
            cout<<"main: producer_"<<i<<" joined with "<<prod_results[i]<<endl;
        }
        i++;
    }

    print_statistics(*share);
    
    return 0;
}
