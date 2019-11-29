#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

using std::vector;
using std::queue;
using std::mutex;
using std::condition_variable;
using std::future;
using std::thread;
using std::function;
using std::unique_lock;
using std::move;
using std::make_shared;
using std::bind;
using std::packaged_task;


class thread_pool
{
private: 
    vector<thread> thread_vec;
    queue<function<void()>> task_queue;
    mutex queue_mtx;
    condition_variable condition;
    bool stop;
public:
    thread_pool(size_t size);
    ~thread_pool();
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args)
    ->future<typename std::result_of<F(Args...)>::type>;
};

inline thread_pool::thread_pool(size_t size)
:stop(false)
{
    for(int i = 0; i < size; ++i)
    {
        thread_vec.emplace_back(
            [this]
            {
                for(;;)
                {
                    function<void()>task;
                    {
                        unique_lock<mutex> lck(queue_mtx);
                        condition.wait(lck,[this]{return stop || !task_queue.empty();});
                        if(stop && task_queue.empty())
                            return;
                        task = std::move(task_queue.front());
                        task_queue.pop();
                    }
                    task();
                }
            }
        );
    }
}

template<typename F, typename... Args>
auto thread_pool::enqueue(F&& f, Args&&... args)
    ->future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = make_shared<packaged_task<return_type()>>(
        bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    future<return_type> res = task->get_future();
    {
        unique_lock<mutex> lck(queue_mtx);
        if(stop)    
            throw std::runtime_error("enqueue on stopped thread_pool.");
        task_queue.emplace([task]{(*task)();});
    }
    condition.notify_one();
    return res;
}

thread_pool::~thread_pool()
{
    {
        unique_lock<mutex> lck(queue_mtx);
        stop = true;
    }
    condition.notify_all();
    for(auto& a:thread_vec)
        a.join();
}
/*
class thread_pool
{
private:
    vector<thread> thread_vec;
    queue<function<void()>> task_queue;
    bool stop = false;
    mutex queue_mtx;
    condition_variable condition;
public:
    thread_pool(size_t size);
    ~thread_pool();
    template<typename F,typename... Args>
    auto enqueue(F&& f, Args&&... args)
    ->future<typename std::result_of<F(Args)...>::type>;    //注意:每添加一个任务队列就会返回一个future对象
};                                                          //等线程执行完该任务后，通过这个对象来获取执行结果

inline thread_pool::thread_pool(size_t size)
:stop(false)
{
    for(int i = 0; i < size; ++i)
    {
        thread_vec.emplace_back(
            [this]
            {
                for(;;)
                {
                    function<void()> task;
                    {
                        unique_lock<mutex> lck(queue_mtx);
                        condition.wait(lck,
                        [this]
                        {
                            return stop || !task_queue.empty();
                        });
                        if(stop && task_queue.empty())
                            return;
                        task = std::move(task_queue.front());
                        task_queue.pop();
                    }
                    task();
                }
            }
        );
    }
}

template<typename F, typename... Args>
auto thread_pool::enqueue(F&& f, Args&&... args)
->future<typename std::result_of<F(Args)...>::type>
{
    using return_type = typename std::result_of<F(Args)...>::type;
    auto task = make_shared<packaged_task<return_type()>>(bind(std::forward<F>(f), std::forward<Args>(args)...));
    future<return_type> res = task->get_future();
    {
        unique_lock<mutex> lck(queue_mtx);
        if(stop)
            throw std::runtime_error("enqueue on stopped thread pool.");
        task_queue.emplace([task]{(*task)();});
    }
    return res;
}

thread_pool::~thread_pool()
{
    unique_lock<mutex> lck(queue_mtx);
    stop = true;
    lck.unlock();
    condition.notify_all();
    for(auto& a:thread_vec)
        a.join();
}
*/