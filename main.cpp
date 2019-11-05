#include "StrVec.cpp"
#include <iostream>
using namespace std;

int main()
{
    StrVec test;
    test.push_back("hdhie");
    int cap = test.capacity();
    cout << cap << endl;
    test.push_back("hdhie");
    test.push_back("hdhie");
    test.push_back("hdhie");
    cap = test.capacity();
    cout << cap << endl;
    return 0;
}