#include <iostream>
#include <vector>
#include <chrono>

#include "Thread_pool.h"

int main()
{
    
    ThreadPool pool(4);
    //results中存放的是函数模板的对象
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        //用emplace_back而不用push_back是因为emplace_back的效率更高
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }
//此处使用了右值引用，是因为返回值只能用右值引用来调用
    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    
    return 0;
}