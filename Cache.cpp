//用来写LRU和LFU两种置换算法，也是面试常考题
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

//1.因为要实现O(1)的时间复杂度，双向链表用于表示位置，hash表用于存储和查找，用模板实现，可适配各种数据类型
template<typename Keytype, typename Valuetype>
class LRU
{
    private:
        typedef pair<Keytype, Valuetype> node;
        typedef typename list<node>::iterator iter;
        //用于存储键值对
        list<node> li;
        //用于存储键的位置信息
        unordered_map<Keytype, iter> um;
        int len;
    public:
        LRU(int capacity):len(capacity){}
        void set(const Keytype&, const Valuetype&);
        int get(const Keytype&);
};

template<typename Keytype, typename Valuetype>
void LRU<Keytype, Valuetype>::set(const Keytype& key, const Valuetype& value)//注意:定义成员函数时，需要在类名上加上类型参数
{
    if(um.count(key) > 0)
    {
        (*um[key]).second = value;
        li.splice(li.begin(), li, um[key]);
    }
    else
    {
        li.emplace_front(make_pair(key,value));
        um[key] = li.begin();
    }
    if(li.size() > len)
    {
        um.erase(li.back().first);
        li.pop_back();
    }
    return;
}

template<typename Keytype, typename Valuetype>
int LRU<Keytype, Valuetype>::get(const Keytype& key) 
{
    if(um.count(key) > 0)
    {
        li.splice(li.begin(),li,um[key]);
        return (*um[key]).second;
    }
    else
        return -1;
}


//2.用模板类来实现LFU，三个unordered_map来实现
template<typename Keytype, typename Valuetype>
class LFU
{
    typedef typename list<Keytype>::iterator iter;

    private:
        int cap;  //数据量上限
        int size; //当前数据量
        int minfreq;
        unordered_map<Keytype, pair<Valuetype,int>> record; //记录每个key对应的value和count
        unordered_map<int, list<Keytype>> fre;              //记录每个count值对应的key链表
        unordered_map<Keytype, iter> pos;//记录每个key在链表中的位置
    public:
        LFU(int capacity):cap(capacity), size(0), minfreq(0){};
        int get(const Keytype& key);
        void set(const Keytype& key, const Valuetype& value);
};

template<typename Keytype, typename Valuetype>
int LFU<Keytype, Valuetype>::get(const Keytype& key)
{
    //如果没有该key值
    if(record.count(key) <= 0)
        return -1;
    fre[record[key].second].erase(pos[key]); //更新频率记录
    record[key].second++;
    fre[record[key].second].emplace_back(key);
    pos[key] = --fre[record[key].second].end();//更新位置信息

    if(fre[minfreq].size() == 0)
        ++minfreq;
    return record[key].first;
}

template<typename Keytype, typename Valuetype>
void LFU<Keytype, Valuetype>::set(const Keytype& key, const Valuetype& value)
{
    int flag = get(key);
    //如果已经有该key
    if(flag != -1)
    {
        record[key].first = value;
        return;
    }
    
    if(size >= cap)
    {
        record.erase(fre[minfreq].front());
        pos.erase(fre[minfreq].front());
        fre[minfreq].pop_front();
        --size;
    }
    record[key] = make_pair(value,1);
    fre[1].emplace_back(key);
    pos[key] = --fre[1].end();
    minfreq = 1;
    ++size;
    return;
}

int main()
{
    LFU<int,int> test1(3);
    test1.set(1,1);
    test1.set(2,2);
    cout << test1.get(1) << endl;
    test1.set(3,3);
    cout << test1.get(2) << endl;
    cout << test1.get(3) << endl;
    test1.set(4,4);
    cout << test1.get(1) << endl;
    cout << test1.get(3) << endl;
    cout << test1.get(4) << endl;
    return 0;
}