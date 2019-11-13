//约瑟夫环问题的解法有两个
//第一个是用环形链表进行依次删除的操作，最后剩下一个答案节点
//第二个是用数学公式推导的方法，递推公式为f(n,m) = (f(n-1,m)+m)%i;搞懂原理
#include <iostream>
using namespace std;
int josph(int n, int m)
{
    int res = 0;
    for(int i = 2; i <= n; ++i)
        res = (res + m) % i;
    return res;
}
int main()
{
    int n,m;
    cin >> n >> m;
    cout << josph(n,m)+1 << endl;
    return 0;
}