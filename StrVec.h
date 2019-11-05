//自己写的一个关于string的类，底层用vector来存储，主要关注的是内存的分配策略
//首先，需要分配器来为分配内存，那么肯定也会用到迭代器(这里用指针来代替)
//其次，要格外主要元素的销毁和内存的分配和释放

#include <vector>
#include <string>
#include <utility>
#include <memory>
using std::string;
class StrVec
{
//需要的私有成员有:指向头元素的指针，指向尾元素后一位的指针，指向最后的指针
//元素释放的函数、分配器、内存扩大函数、二次分配函数以及元素指针对都不应该暴露给用户
private:
    string* elements;
    string* first_free;
    string* cap;
    void free();
    void reallocate();
    std::pair<string*, string*> alloc_n_copy(const string*, const string*);
    void check_n_alloc(){if(size() == capacity()) reallocate();}
    std::allocator<string> alloc;
//公有成员应该有:默认构造函数，拷贝构造函数，析构函数，拷贝赋值运算符，增加元素的函数，析构函数，vector大小返回函数
//容量返回函数，头尾迭代器返回函数
public:
    StrVec():elements(nullptr), first_free(nullptr), cap(nullptr){};
    StrVec(const StrVec&);
    ~StrVec(){ free(); };
    StrVec& operator= (const StrVec&);
    void push_back(const string&);
    size_t size()const {return first_free - elements;}
    size_t capacity()const {return cap - elements;}
    string* begin() const {return elements;}
    string* end() const {return first_free;}

};