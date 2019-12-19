#include <mutex>
#include <atomic>

using std::mutex;
using std::unique_lock;
using std::atomic;

//记录单例模式的几种写法
//单例模式:保证一个类仅有一个实例，并提供该实例的一个全局访问点

class singleton
{
    //由于不能构造其他实例，因此将构造函数和拷贝构造函数放在私有成员函数中
private:
    singleton();
    singleton(const singleton& other);
    
public:
    //因为要全局访问，所以要用static来修饰获取函数以及被获取的对象指针
    static singleton* get_instance();
    static singleton* m_instance;
    static atomic<singleton*> m_instance1;
    static mutex m_mtx;
};

//初始化静态变量
/*
singleton* singleton::m_instance = nullptr;
mutex singleton::m_mtx;
*/

//非线程安全版本，这种版本在单线程的环境中可以正常工作，但非线程安全，这种写法叫懒汉单例模式
//即在使用时才实例化出对象，对应的饿汉单例模式是一开始就实例化出对象
/*
singleton* singleton::get_instance()
{
    if(m_instance == nullptr)
        m_instance = new singleton();
    return m_instance;
}
*/

//加锁式多线程版本，这种写法是线程安全的，但加锁成本很高，不适合高并发场景下
//实例化之后的每次调用都还会进行加锁，这部分开销是多余的
/*
singleton* singleton::get_instance()
{
    std::unique_lock<mutex> lck(m_mtx);
    if(m_instance == nullptr)
        m_instance = new singleton();
    return m_instance;
}
*/


//double_check+lock的方法，该写法看起来很正确，但实际上有问题，不能使用
//问题出在编译器的优化处理上，由于编译器在执行优化时可能会reorder代码的执行顺序，所以还是可能出问题
//例如:m_instance = new singleton()，本来应该是先分配内存，然后调用构造器，最后进行赋值操作，但reorder之后可能变成
//先分配内存，然后赋值，最后调用构造器，这就可能导致其他线程拿到一块没有进行构造的内存
/*
singleton* singleton::get_instance()
{
    if(m_instance == nullptr)
    {
        unique_lock<mutex> lck(m_mtx);
        if(m_instance == nullptr)
            m_instance = new singleton();
    }
    return m_instance;
}
*/

//运用c++11中的多线程库可以实现真正的线程安全且没有多余开销的版本，注:volatile不是跨平台的
//这种方法既没有像饿汉式提前占用空间资源，也不会像懒汉式一样产生过多加锁的开销，是最好的写法
/*
atomic<singleton*> singleton::m_instance1;
mutex singleton::m_mtx;

singleton* singleton::get_instance()
{
    singleton* tmp = m_instance1.load(std::memory_order_relaxed);
    //加内存屏障，防止编译器做优化导致reorder的问题
    std::atomic_thread_fence(std::memory_order_acquire);
    if(tmp == nullptr)
    {
        unique_lock<mutex> lck(m_mtx);
        tmp = m_instance1.load(std::memory_order_relaxed);
        if(tmp == nullptr)
        {
            tmp = new singleton();
            std::atomic_thread_fence(std::memory_order_release);
            m_instance1.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}
*/


//饿汉单例模式
//访问量较小时用懒汉，时间换空间
//由于要进行线程同步，所以高并发时采用饿汉，空间换时间
class singleton
{
private:
    singleton();
    singleton(const singleton& s);
    static singleton* m_instance;
public:
    static singleton* get_instance();
};
singleton* singleton::m_instance = new singleton();
singleton* singleton::get_instance()
{
    return m_instance;
}
