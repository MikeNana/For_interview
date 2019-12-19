//实现无锁队列有两个关键点
//1.各个平台的原子操作或者说CAS原语(comare and swap)比较和旧值是否相同，相同则置为新值，否则不做任何操作
//2.ABA问题的理解和解决

//linux内核中的kfifo实现了单生产者单消费者的无锁队列，该种实现是真正意义上的无锁，其中采用的数学知识和内存屏障保证了超高的效率
//boost内部实现的几种无锁队列不是真正意义上的无锁，只是采用了轻量级的原子锁
//java中实现无锁队列相对简单，因为有垃圾回收机制，不用考虑ABA问题，而在c++中要自己实现无锁队列，需要考虑这个问题

#include <string>
#include <atomic>
using std::string;
struct LinkNode
{
    string data;
    LinkNode* next;
};

typedef struct LinkNode LinkNode;
class lock_free_queue
{
public:
    lock_free_queue();
    ~lock_free_queue();
    int push(const string& a);
    string saferead(LinkNode* q);
private:
    LinkNode* head;
    LinkNode* tail;
    bool empty_;
    unsigned int length_;
};

lock_free_queue::lock_free_queue():head(nullptr), tail(nullptr),empty_(true),length_(0)
{
    head = new LinkNode();
    head->next = nullptr;
    tail = head;
}
lock_free_queue::~lock_free_queue()
{
    LinkNode* p = head;
    while(p)
    {
        LinkNode* q = p->next;
        delete p;
        p = q;
    }
}

int lock_free_queue::push(const string& a)
{
    LinkNode* q = new LinkNode();
    q->data = a;
    q->next = nullptr;
    LinkNode* p = tail;
    LinkNode* oldp = p;
    do
    {
        while(p->next)
            p = p->next;
    } while (__sync_bool_compare_and_swap(&(p->next), NULL ,q) != true);
    __sync_bool_compare_and_swap(&tail, oldp ,q);
}

string lock_free_queue::saferead(LinkNode* q)
{
    LinkNode* p;
    do
    {
        p = head;
        if(p->next == nullptr)
            return "";
    } while (__sync_bool_compare_and_swap(&head, p, p->next) != true );
    return p->next->data;
    
}

//这种写法没有考虑ABA问题，要使用无锁队列还是直接用boost库中的吧，或者给节点中加版本号，来辨别是否是重复访问