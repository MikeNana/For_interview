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
    vector<thread> thread_vec;                  //存储线程
    queue<function<void()> > task_queue;         //任务队列，线程从队列中取出任务并完成
    bool stop;                                  //标识线程池是否要关闭
    mutex queue_mtx;                            //针对队列的互斥量
    condition_variable condition;             //如果任务队列不为空，则唤醒所有线程
public:
    thread_pool(size_t size);                   //构造函数，只接受单个参数的形式

    template<typename F, typename... Args>      //成员模板函数，将可调用对象的任务加入到任务队列中
    auto enqueue(F&& f, Args&&... args)         //用异步返回的方式接收返回值
        ->future<typename std::result_of<F(Args...)>::type>;

    ~thread_pool();
};

inline thread_pool::thread_pool(size_t size):stop(false)
{
    for(size_t i = 0; i < size; ++i)
    {
        thread_vec.emplace_back
        (
            [this]
            {
                for(;;)
                {
                    function<void()> task;
                    {
                        unique_lock<mutex> lck(queue_mtx);
                        condition.wait(lck,[this]{return stop || !task_queue.empty();});
                        if(stop && task_queue.empty())
                            return;
                        task = move(task_queue.front());
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
        bind(std::forward<F>(f), std::forward<Args>(args)...)       //用make_shared + forward的方式实现完美转发
        );                                                          //将要处理的任务作为参数完美转发给变量task，方便存储
    std::future<return_type> res = task->get_future();              //将结果异步返回，中间的处理过程交给线程池中的线程
    //这里放大括号是为了让锁资源自己释放
    {
        unique_lock<mutex> lck(queue_mtx);
        //如果准备停止线程池，则不再放入任务队列
        if(stop)
            throw std::runtime_error("enqueue on stopped threaad_pool.\n");
        task_queue.emplace([task](){(*task)();});
    } 
    //当任务队列不为空时，唤醒其他线程拿走任务并执行
    condition.notify_all();
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