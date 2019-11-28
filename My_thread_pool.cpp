#include <chrono>
#include <vector>
#include <iostream>
#include <future>

#include "My_thread_pool.h"
using std::thread;
using std::vector;
using std::future;
using std::cout;
using std::endl;

int main()
{
    thread_pool threads(4);
    vector<future<int>> results;
    for(int i = 0; i < 8; ++i)
    {
        results.emplace_back(
            threads.enqueue([i]{
                cout << "hello" << i << endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                cout << "world" << i << endl;
                return i*i;
            })
        );
    }
    for(auto&& a:results)
        cout << a.get() << ' ';
    cout << endl;
    return 0;
}