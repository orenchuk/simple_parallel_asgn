#include <iostream>
#include <math.h>
#include <chrono>
#include <atomic>

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

int calc() {
    int count = 0, from = 0, to = 200;
    
    for (int w = from; w <= to; ++w) {
//        cout << "-";
        for (int x = from; x <= to; ++x) {
            for (int y = from; y <= to; ++y) {
                for (int z = from; z <= to; ++z) {
                    int a = pow(w, 3), b = pow(x, 3), c = pow(y, 3), d = pow(z, 3);
                    if ((a + b == c + d) && (a != c) && (b != d) && (a != d) && (b != c)) {
//                        cout << endl << a << " + " << b << " == " << c << " + " << d;
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}
int main(int argc, char *argv[]) {
    int result;
    auto start_time = get_current_time();
    result = calc();
    auto finish_time = get_current_time();
    auto total_time = finish_time - start_time;
    cout << "Amount: " << result << endl;
    cout << "time: " << time_ms(total_time) << "ms" << endl;
    return 0;
}
