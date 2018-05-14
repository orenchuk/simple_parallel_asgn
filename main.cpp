#include <iostream>
#include <math.h>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <future>

using std::cout;
using std::endl;

inline std::chrono::high_resolution_clock::time_point get_current_time() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template <class T>
inline long long time_ms(const T& t) {
    return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
}

void calc(int local_from, int local_to, int max, std::promise<int> result) {
    int count = 0, from = 0, to = max + 1;
    for (int w = local_from; w < local_to; ++w) {
        for (int x = from; x < to; ++x) {
            for (int y = from; y < to; ++y) {
                for (int z = from; z < to; ++z) {
                    int a = pow(w, 3), b = pow(x, 3), c = pow(y, 3), d = pow(z, 3);
                    if ((a + b == c + d) && (a != c) && (b != d) && (a != d) && (b != c)) {
                        ++count;
                    }
                }
            }
        }
    }
    try {
        result.set_value(count);
    } catch (...) {
        result.set_exception(std::current_exception());
    }
}

int main(int argc, const char * argv[]) {
    const int max = 200;
    int n_threads = 4;
    const int step = max / n_threads;
    int from = 0, to = from + step;
    int count = 0;
    
    if (argc > 1) {
        if (atoi(argv[1]) < max) {
            n_threads = atoi(argv[1]);
        } else {
            n_threads = max;
        }
    }
    cout << "Threads: " << n_threads << endl;
    cout << "Working..." << endl;
    
    std::vector<std::thread> threads;
    
    auto start_time = get_current_time();
    
    try {
        std::vector<std::future<int>> futures;
        for (int i = 0; i < n_threads; ++i) {
            cout << "from: " << from << " to: " << to << endl;
            std::promise<int> promises;
            futures.push_back(promises.get_future());
            threads.emplace_back(calc, from, to, max, std::move(promises));
            from = to;
            to += step;
            if (i == n_threads - 2) { to = max + 1; }
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        for (auto& future : futures) {
            count += future.get();
        }
        
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << endl;
    }
    
    cout << "Amount: " << count << endl;
    
    auto finish_time = get_current_time();
    auto total_time = finish_time - start_time;
    cout << "time: " << time_ms(total_time) << "ms" << endl;
    
    return 0;
}
