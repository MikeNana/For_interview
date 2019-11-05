//本文件用于记录常见的可调用对象，包括普通函数，类成员函数，类静态函数，仿函数，函数指针，std::funciton，lambda表达式

#include <bits/stdc++.h>
using namespace std;
//1.普通函数
int normal_func(int x, int y)
{
    return x+y;
}

//2.类成员函数以及静态函数
class test
{
    public:
        int base = 10;
        //普通成员函数
        int add0(int x, int y){return this->base + x + y;}
        //静态成员函数，没有this指针
        static int add1(int x, int y){return x+y;}
};

//3.仿函数，使用类来模拟函数的行为，通过在类中重载operator()函数，可以像调用一个函数一样调用一个类
//仿函数的调用速度比普通函数快,性能更优，可以一定程度上解决参数数量的问题，逻辑与迭代可分开，并且可以保持状态
class shorterthan
{
    private:
        int len;
    public:
        shorterthan(int maxlength):len(maxlength){}
        bool operator()(const string& str)
        {
            return str.size() > len;
        };
};

//4.函数指针，即指向函数的指针，其他地方可以通过调用该指针来调用指针指向的函数，是标准的c++回调函数的解决方案
//被调用的三个函数
int add(int x, int y)
{
    return x+y;
}
int minu(int x, int y)
{
    return x-y;
}
int multiple(int x, int y)
{
    return x*y;
}
//定义一个回调函数
int compute(int x, int y, int (*call_back)(int, int))//call_back是函数指针
{
    return call_back(x,y);
}

//5.c++11新标准，lambda表达式，非常好用，示例用“打印string数组中大于某个长度的所有单词”
void biggies(vector<string>& str, string::size_type sz)
{
    //先将单词按字典序排序并删除相同的
    sort(str.begin(),str.end());
    str.erase(unique(str.begin(), str.end()), str.end());
    //按长度排序，并保持字典序
    stable_sort(str.begin(),str.end(), [](const string& a, const string& b){return a.size() < b.size();});
    //找到长度比sz长的字符位置
    auto pos = find_if(str.begin(), str.end(), [sz](const string& a){return a.size() >= sz;});
    //将所有的打印出来
    for_each(pos, str.end(),[](const string& s){cout << s << " ";});
    return;
}

//6.c++11新标准，std::function模板，主要作用是对函数进行包装，它可以兼容所有具有相同参数的函数实体(包括之前总结的所有种类)
std::function<int(int,int)> sumfunction;
//普通函数
int sum0(int x, int y)
{
    return x+y;
}
//类的成员函数和静态函数
class sum_class
{
    private:
        int index = 10;
    public:
        int sum1(int x, int y){return this->index + x + y;}
        static int sum2(int x, int y){return x+y;}
};
//仿函数
class sum_functor
{
    public:
        int operator()(int x, int y){return x+y;}
};
//函数指针
int (*function_ptr)(int x, int y);
//lambda表达式
auto lambda_sum = [](int x, int y){return x+y;};
int main()
{
    int x = 10, y = 15;
    //普通函数调用
    cout << normal_func(x,y) << endl;
    //类成员函数/静态函数调用
    test test1;
    cout << test1.add0(1,9) << endl;
    cout << test1.add1(1,9) << endl;
    //仿函数调用
    vector<string> strvec{"sfsdf", "skfjskjlfa", "sdfaf", "sadgadgsd"};
    shorterthan test2(2);
    cout << count_if(strvec.begin(),strvec.end(), test2) << endl;
    //回调函数，函数指针
    cout << compute(12, 14, add) << endl;
    cout << compute(12, 14, minu) << endl;
    cout << compute(12, 14, multiple) << endl;
    //lambda表达式
    biggies(strvec, 6);
    //std::function函数模板
    sumfunction = sum0;
    cout << sumfunction(2, 9) << endl;//11
    sum_class sum_obj;
    //普通的类成员函数需要绑定this对象，但是静态成员函数不用，毕竟静态成员函数也没有this指针
    sumfunction = bind(&sum_class::sum1, sum_obj, std::placeholders::_1, std::placeholders::_2);
    cout << sumfunction(2, 18) << endl;//30
    //类静态函数不用绑定this，
    sumfunction = sum_class::sum2;
    cout << sumfunction(2, 18) << endl;//20
    return 0;
}