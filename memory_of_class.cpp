// 该文件用于解释对于类对象的内存分配规则，即对对象使用sizeof得到的结果
// 一个类对象中，虚函数(虚函数指针占空间)、成员函数（包括静态与非静态）和静态数据成员都是不占用类对象的存储空间的。 
// 对象大小= vptr(可能不止一个，这个很难确定，不过试过，类中定义了一个virtual函数，仍然为占用4个字节) 
//         + 所有非静态数据成员大小 
//         + Aligin字节大小（依赖于不同的编译器）
// 1.为类的非静态成员数据的类型大小之和
// 2.由编译器额外加入的成员变量的大小，用来支持语言的某些特性（如：指向虚函数的指针）
// 3.为了优化存取效率，进行的边缘调整（字节对齐）
// 4.与类中的构造函数，析构函数以及其他的成员函数无关
#include <bits/stdc++.h>
using namespace std;
class A
{

};
class B
{
    char a;
    char b;
    int c;
};
class C
{
    char a;
    char b;
    int c;
    void func(){};
};
class D
{
    char a;
    char b;
    int c;
    virtual void func(){};
};
class E: public A, public B
{

};
class F: public C, public D
{

};
int main()
{
    A a;
    B b;
    C c;
    D d;
    E e;
    F f;
    cout << sizeof(a) << endl;
    cout << sizeof(b) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(d) << endl;
    cout << sizeof(e) << endl;
    cout << sizeof(f) << endl;
    return 0;
}