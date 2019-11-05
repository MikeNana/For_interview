#include <algorithm>
#include <utility>
#include "StrVec.h"
    string* elements;
    string* first_free;
    string* cap;

void StrVec::free()
{
    if(elements)
    {
        //逆序删除元素
        for(auto a = first_free; a != elements; --a)
            alloc.destroy(a);
        //释放之前分配的空间
        alloc.deallocate(elements, cap-elements);
    }
    return;
}

void StrVec::reallocate()
{
    auto newcapacity = size() ? 2*size() : 1;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto ele = elements;
    for(size_t i = 0; i < size(); ++i)
        alloc.construct(dest++, std::move(*ele++));
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}
//主要是为了拷贝构造函数服务
std::pair<string*, string*> StrVec::alloc_n_copy(const string* b, const string* e)
{
    auto data = alloc.allocate(e - b);
    return {data, uninitialized_copy(b, e, data)};
}

StrVec::StrVec(const StrVec& a)
{
    auto newdata = alloc_n_copy(a.begin(), a.end());
    elements = newdata.first;
    first_free = newdata.second;
}
StrVec& StrVec::operator= (const StrVec& s)
{
    auto data = alloc_n_copy(s.begin(), s.end());
    free();
    elements = data.first;
    first_free = data.second;
    cap = first_free;
    return *this;
}

void StrVec::push_back(const string& s)
{
    check_n_alloc();
    alloc.construct(first_free++, s);
}