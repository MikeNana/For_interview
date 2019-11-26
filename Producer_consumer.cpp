//记录生产者/消费者的解决方案
//分为四种情况，单生产者/单消费者，单生产者/多消费者，多生产者/单消费者，多生产者/多消费者
//所有的示例中使用的都是int类型的"产品"，可替换成其他的任何形式


#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <iterator>

using std::mutex;
using std::condition_variable;
using std::cout;
using std::endl;
using std::unique_lock;
using std::thread;
using std::vector;
//1.单生产者/单消费者只有一个生产者和一个消费者，生产者向产品库中放置产品，消费者从产品库中取走产品，产品库缓冲区的容量有一定限制
//如果生产者生产过快，则需要等待消费者取走产品之后才能继续生产
//如果消费者消费过快，则需要等待生产者放置产品之后才能继续消费
//通过对产品库加锁实现
/*
static const int kItemToProduce = 1000; //要创建的产品量
static const int BufferSize = 10;       //循环队列的缓冲区大小

struct itemReposity 
{
    int item_buffer[BufferSize];  //用一个循环队列模拟缓冲区，可以防止溢出
    size_t read_position;               //消费者即将要读的位置
    size_t write_position;              //生产者即将要写的位置
    mutex mtx;                          //互斥量，保护产品缓冲区
    condition_variable repo_not_full;   //条件变量，标识缓冲区不为满
    condition_variable repo_not_empty;  //条件变量，标识缓冲区不为空
}gItemToProduce;                        //产品库全局变量，生产者和消费者操作这个变量

typedef struct itemReposity itemReposity;

void produce_item(itemReposity* it, int item)  
{
    unique_lock<mutex> lck(it->mtx);    //首先对该产品库加锁
    while(((it->write_position+1) % BufferSize) == it->read_position) //用while而不是if的原因是，如果有多个消费者和生产者，再次被唤醒时有可能不满足条件
    {
        cout << "Producer is waiting for an empty slot.\n";
        (it->repo_not_full).wait(lck);    //等待缓冲区有空位出现，被唤醒
    }
    //当条件满足之后，更新相关变量
    (it->item_buffer)[it->write_position] = item; //读取当前生产的产品并放入队列
    (it->write_position)++;                       //更新位置
    if(it->write_position == BufferSize)    //如果下一个位置在队列末尾后面，则将位置重新置为0
        it->write_position = 0;                 
    (it->repo_not_empty).notify_all();            //唤醒正在等待队列不为空的线程
    lck.unlock();                               //释放掉互斥量，让其他处于wait的线程竞争锁资源
}

int consume_item(itemReposity* it)
{
    int data;
    unique_lock<mutex> lck(it->mtx);
    while(it->read_position == it->write_position)  //等待任务队列不为空
    {
        cout << "Consumer is waiting for new items...\n";
        (it->repo_not_empty).wait(lck);
    }

    data = (it->item_buffer)[it->read_position];
    (it->read_position)++;
    if(it->read_position >= BufferSize)
        it->read_position = 0;
    (it->repo_not_full).notify_all();
    lck.unlock();

    return data;
}

void ProduceTask()
{
    for(int i = 1; i <= kItemToProduce; ++i)
    {
        cout << "Produce " << i << " ^th item..." << endl; 
        produce_item(&gItemToProduce, i);
    }
}

void ConsumeTask()
{
    static int consume_cnt = 0;             //记录消费了多少个产品
    while(1)
    {
        sleep(1);
        int item = consume_item(&gItemToProduce);
        cout << "Consume the " << item << " ^th item" << endl;
        if(++consume_cnt == kItemToProduce)
            break;
    }
}

void initReposity(itemReposity* it)
{
    it->read_position = 0;
    it->write_position = 0;
}

int main()
{
    initReposity(&gItemToProduce);

    thread producer_thread(ProduceTask);
    thread consumer_thread(ConsumeTask);
    consumer_thread.join();
    producer_thread.join();

    return 0;
}
*/


//2.单生产者/多消费者模型，包含一个生产者和多个消费者，允许多个消费者同时从产品库中取走产品
//所以除了要保证产品库在多个读写线程下的互斥以外，还要维护消费者的计数器
/*
static const int kitemtoproducer = 10;
static const int buffersize = 4;

struct itemrepository
{
    int buffer[buffersize];
    mutex mtx;
    mutex mtx_cnt;
    condition_variable repo_not_full;
    condition_variable repo_not_empty;
    size_t read_position;
    size_t write_position;
    size_t consume_cnt;
}repository;

typedef struct itemrepository itemrepository;

void produce_item(itemrepository* it, int item)
{
    unique_lock<mutex> lck(it->mtx);
    while((it->write_position+1) % buffersize == it->read_position)
    {
        cout << "Producer is waiting for an empty slot.\n";
        it->repo_not_full.wait(lck);
    }

    it->buffer[it->write_position] = item;
    it->write_position++;
    if(it->write_position == buffersize)
        it->write_position = 0;
    it->repo_not_empty.notify_all();
    lck.unlock();
}

int consume_item(itemrepository* it)
{
    int data;
    unique_lock<mutex> lck(it->mtx);
    while(it->write_position == it->read_position)
    {
        cout << "Consumer is waiting for new items.\n";
        it->repo_not_empty.wait(lck);
    }
    data = it->buffer[it->read_position];
    it->read_position++;
    if(it->read_position >= buffersize)
        it->read_position = 0;
    it->repo_not_full.notify_all();
    lck.unlock();
    return data;
}

void ProducerTask()
{
    for(int i = 0; i < kitemtoproducer; ++i)
    {
        cout << "Thread " << std::this_thread::get_id() << " is producing " << i+1 << "^th cargo.\n";
        produce_item(&repository, i+1);
    }
    cout << "Thread " << std::this_thread::get_id() << " is exiting.\n";
}

void ConsumerTask()
{
    bool ready_to_exit = false;
    while(1)
    {
        //sleep(1);
        unique_lock<mutex> lck_cnt(repository.mtx_cnt);
        if(repository.consume_cnt < kitemtoproducer)
        {
            int item = consume_item(&repository);
            cout << "Consume thread " << std::this_thread::get_id() << " is consuming " << item << "^th item.\n";
            ++repository.consume_cnt;
        }
        else
            ready_to_exit = true;
        lck_cnt.unlock();
        if(ready_to_exit)
            break;
    }
}

void init_repository(itemrepository* it)
{
    it->read_position = 0;
    it->write_position = 0;
    it->consume_cnt = 0;
}

int main()
{
    vector<thread> consume_thread;
    thread producer_thread(ProducerTask);
    for(int i = 0; i < 4; ++i)
        consume_thread.emplace_back(thread(ConsumerTask));
    producer_thread.join();
    for(auto& a:consume_thread)
        a.join();
    return 0;
}
*/

//3.多生产者/单消费者模型，包含多个生产者和一个消费者，允许多个生产者同时生产
//只需多维护一个生产者的计数器即可
/*
static const int k_to_produce = 10;
static const int buffersize = 4;

struct repository
{
    int buffer[buffersize];
    mutex mtx;
    mutex mtx_produce;
    size_t read_position;
    size_t write_position;
    size_t produce_cnt;
    condition_variable repo_not_full;
    condition_variable repo_not_empty;
    
}itemrepository;

typedef repository repository;

void produce_item(repository* it, int item)
{
    unique_lock<mutex> lck(it->mtx);
    while((it->write_position+1) % buffersize == it->read_position)
    {
        cout << "Producer thread is waiting for an empty slot.\n";
        it->repo_not_full.wait(lck);
    }
    it->buffer[it->write_position++] = item;
    if(it->write_position == buffersize)
        it->write_position = 0;
    it->repo_not_empty.notify_all();
    lck.unlock();
}
int consume_item(repository* it)
{
    int data;
    unique_lock<mutex> lck(it->mtx);
    while(it->write_position == it->read_position)
    {
        cout << "Consume thread is waiting for new items.\n";
        it->repo_not_empty.wait(lck);
    }
    data = it->buffer[it->read_position++];
    if(it->read_position == buffersize)
        it->read_position = 0;
    it->repo_not_full.notify_all();
    lck.unlock();
    return data;
}
void Produce_task()
{
    bool ready_to_exit = false;
    while(1)
    {
        unique_lock<mutex> lck_cnt(itemrepository.mtx_produce);
        if(itemrepository.produce_cnt < k_to_produce)
        {
            ++itemrepository.produce_cnt;
            produce_item(&itemrepository, itemrepository.produce_cnt);
            cout << "Thread " << std::this_thread::get_id() << " is producing " << itemrepository.produce_cnt
            << " ^th cargo." << endl;
        }
        else
            ready_to_exit = true;
        lck_cnt.unlock();
        if(ready_to_exit)
            break;
    }
    cout << "Thread " << std::this_thread::get_id() << " is exiting." << endl;
}
void Consume_task()
{
    bool ready_to_exit = false;
    static int item_consumed = 0;
    while(1)
    {
        sleep(1);
        if(item_consumed < k_to_produce)
        {
            int item = consume_item(&itemrepository);
            cout << "Thread " << std::this_thread::get_id() << " is consuming " << item
            << " ^th cargo." << endl;
            ++item_consumed;
        }
        else
            ready_to_exit = true;
        if(ready_to_exit)
            break;
    }
    cout << "Thread " << std::this_thread::get_id() << " is exiting..." << endl;
}

void init_itemrepository(repository* it)
{
    it->write_position = 0;
    it->read_position = 0;
    it->produce_cnt = 0;
}

int main()
{
    vector<thread> produce_thread;
    for(int i = 0; i < 4; ++i)
        produce_thread.emplace_back(thread(Produce_task));
    thread consume_thread(Consume_task);
    consume_thread.join();
    for(auto& a:produce_thread)
        a.join();
    return 0;
}
*/

//4.多生产者/多消费者模型，包含多个生产者和多个消费者，允许多个生产者同时生产，多个消费者同时消费
//需要维护两个计数器分别表示生产者生产的数量和消费者消费的数量
static const int k_to_produce = 10;
static const int buffersize = 4;

struct repository
{
    int buffer[buffersize];
    size_t read_position;
    size_t write_position;
    size_t produce_cnt;
    size_t consume_cnt;
    mutex mtx;
    mutex consume_mtx;
    mutex produce_mtx;
    condition_variable repo_not_full;
    condition_variable repo_not_empty;
}item_repository;

typedef struct repository repository;

void produce_item(repository* it, int item)
{
    unique_lock<mutex> lck(it->mtx);
    while((it->write_position+1) % buffersize == it->read_position)
    {
        cout << "Produce thread is waiting for an empty slot." << endl;
        it->repo_not_full.wait(lck);
    }
    it->buffer[it->write_position++] = item;
    if(it->write_position == buffersize)
        it->write_position = 0;
    it->repo_not_empty.notify_all();
    lck.unlock();
}
int consume_item(repository* it)
{
    int data;
    unique_lock<mutex> lck(it->mtx);
    while(it->write_position == it->read_position)
    {
        cout << "Consume thread is waiting for new items." << endl;
        it->repo_not_empty.wait(lck);
    }
    data = it->buffer[it->read_position++];
    if(it->read_position == buffersize)
        it->read_position = 0;
    it->repo_not_full.notify_all();
    lck.unlock();
    return data;
}
void produce_task()
{
    bool ready_to_exit = false;
    while(1)
    {
        sleep(1);
        unique_lock<mutex> lck_produce(item_repository.produce_mtx);
        if(item_repository.produce_cnt < k_to_produce)
        {
            ++item_repository.produce_cnt;
            produce_item(&item_repository, item_repository.produce_cnt);
            cout << "Thread " << std::this_thread::get_id() << " is producing " << item_repository.produce_cnt
            << "^th item." << endl;
        }
        else
            ready_to_exit = true;
        lck_produce.unlock();
        if(ready_to_exit)
            break;
    }
    cout << "Thread " << std::this_thread::get_id() << " is exiting." << endl;
}
void consume_task()
{
    bool ready_to_exit = false;
    while(1)
    {
        int data;
        sleep(1);
        unique_lock<mutex> lck_consume(item_repository.consume_mtx);
        if(item_repository.consume_cnt < k_to_produce)
        {
            ++item_repository.consume_cnt;
            data = consume_item(&item_repository);
            cout << "Thread " << std::this_thread::get_id() << " is consuming " << data << "^th item." << endl;
        }
        else
            ready_to_exit = true;
        lck_consume.unlock();
        if(ready_to_exit)
            break;
    }
    cout << "Thread " << std::this_thread::get_id() << " is exiting." << endl;
}

void init(repository* it)
{
    it->consume_cnt = 0;
    it->produce_cnt = 0;
    it->read_position = 0;
    it->write_position = 0;
}

int main()
{
    vector<thread> consume_threads;
    vector<thread> produce_threads;
    for(int i = 0; i < 2; ++i)
        produce_threads.emplace_back(thread(produce_task));
    for(int i = 0; i < 2; ++i)
        consume_threads.emplace_back(thread(consume_task));
    for(auto& a:produce_threads)
        a.join();
    for(auto& b:consume_threads)
        b.join();
    return 0;
}

