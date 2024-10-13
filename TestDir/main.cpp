#include "word_count.h"

void producer(std::shared_ptr<SharedObject> so, int *ret) {
    int i = 0;
    std::string line;

    while (true) {
        std::unique_lock<std::mutex> ulock(so->lock);
        so->cond_var.wait(ulock, [so]() { return !so->full; });

        if (!std::getline(so->rfile, line)) {
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

    *ret = i;  // 생산자가 읽은 라인의 개수 반환
    std::cout << "Prod_" << std::this_thread::get_id() << ": " << i << " lines\n";
}

void consumer(std::shared_ptr<SharedObject> so, int *ret) {
    int i = 0;

    while (true) {
        std::unique_lock<std::mutex> ulock(so->lock);
        so->cond_var.wait(ulock, [so]() { return so->full || so->finished; });

        if (so->finished && !so->full) {
            break;
        }

        // 데이터를 처리
        process_line(*so, so->line);
        i++;

        so->full = false;
        so->cond_var.notify_all();
    }

    *ret = i;  // 소비자가 처리한 라인의 개수 반환
    std::cout << "Cons_" << std::this_thread::get_id() << ": " << i << " lines\n";
}
int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "usage: ./prod_cons <readfile> #Producer #Consumer\n";
        return 1;
    }

    int Nprod = (argc > 2) ? std::min(atoi(argv[2]), 100) : 1;
    int Ncons = (argc > 3) ? std::min(atoi(argv[3]), 100) : 1;

    auto share = std::make_shared<SharedObject>();
    share->rfile.open(argv[1]);

    if (!share->rfile.is_open()) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    std::vector<int> prod_results(Nprod);
    std::vector<int> cons_results(Ncons);
    int i = 0;

    // 생산자 스레드 생성
    for (int i = 0; i < Nprod; ++i) {
        producers.emplace_back(producer, share, &prod_results[i]);
    }

    // 소비자 스레드 생성
    for (int i = 0; i < Ncons; ++i) {
        consumers.emplace_back(consumer, share, &cons_results[i]);
    }

    // 소비자 스레드 종료 대기
    for (auto& th : consumers) {
        if (th.joinable()) {
            th.join();
            std::cout << "main: consumer_" << i << " joined with " << cons_results[i] << " lines processed\n";
        }
        i++;
    }

    i = 0;

    // 생산자 스레드 종료 대기
    for (auto& th : producers) {
        if (th.joinable()) {
            th.join();
            std::cout << "main: producer_" << i << " joined with " << prod_results[i] << " lines processed\n";
        }
        i++;
    }

    // 최종 통계 출력
    int sum = 0;
    for (i = 0; i < MAX_STRING_LENGTH; i++) {
        sum += share->stat[i];
    }

    // 문자열 길이에 따른 빈도수 분포 출력
    std::cout << "\n*** print out distributions ***\n";
    std::cout << "  #ch  freq \n";
    for (i = 0; i < MAX_STRING_LENGTH; i++) {
        int j = 0;
        int num_star = (sum > 0) ? (share->stat[i] * 80 / sum) : 0;
        std::cout << "[" << i + 1 << "]: " << share->stat[i] << "\t";
        for (j = 0; j < num_star; j++) {
            std::cout << "*";
        }
        std::cout << "\n";
    }

    // 아스키 문자 빈도수 출력
    std::cout << "       A        B        C        D        E        F        G        H        I        J        K        L        M        N        O        P        Q        R        S        T        U        V        W        X        Y        Z\n";
    std::cout << std::setw(8) << share->stat2['A'] + share->stat2['a'] << std::setw(8) << share->stat2['B'] + share->stat2['b']
              << std::setw(8) << share->stat2['C'] + share->stat2['c'] << std::setw(8) << share->stat2['D'] + share->stat2['d']
              << std::setw(8) << share->stat2['E'] + share->stat2['e'] << std::setw(8) << share->stat2['F'] + share->stat2['f']
              << std::setw(8) << share->stat2['G'] + share->stat2['g'] << std::setw(8) << share->stat2['H'] + share->stat2['h']
              << std::setw(8) << share->stat2['I'] + share->stat2['i'] << std::setw(8) << share->stat2['J'] + share->stat2['j']
              << std::setw(8) << share->stat2['K'] + share->stat2['k'] << std::setw(8) << share->stat2['L'] + share->stat2['l']
              << std::setw(8) << share->stat2['M'] + share->stat2['m'] << std::setw(8) << share->stat2['N'] + share->stat2['n']
              << std::setw(8) << share->stat2['O'] + share->stat2['o'] << std::setw(8) << share->stat2['P'] + share->stat2['p']
              << std::setw(8) << share->stat2['Q'] + share->stat2['q'] << std::setw(8) << share->stat2['R'] + share->stat2['r']
              << std::setw(8) << share->stat2['S'] + share->stat2['s'] << std::setw(8) << share->stat2['T'] + share->stat2['t']
              << std::setw(8) << share->stat2['U'] + share->stat2['u'] << std::setw(8) << share->stat2['V'] + share->stat2['v']
              << std::setw(8) << share->stat2['W'] + share->stat2['w'] << std::setw(8) << share->stat2['X'] + share->stat2['x']
              << std::setw(8) << share->stat2['Y'] + share->stat2['y'] << std::setw(8) << share->stat2['Z'] + share->stat2['z'] << "\n";

    return 0;
}
