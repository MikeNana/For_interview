//完美洗牌算法的初级结论，通过数学推导知道:对于任意的第i个元素，最终换到的位置都是(2*i)%(2n+1)，所以得到初级的洗牌算法
//此时的时间复杂度是O(n),但空间复杂度也是O(n)，所以继续优化
/*
#include <bits/stdc++.h>
using namespace std;
void perfect_shuffle(vector<int>& num, int n)
{
    vector<int> temp(n+1,0);
    for(int i = 1; i < n+1; ++i)
        temp[(2*i)%(n+1)] = num[i];
    for(int i = 1; i < n+1; ++i)
        num[i] = temp[i];
    return;
}
int main()
{
    int n;
    cin >> n;
    vector<int> num(n+1,0);
    for(int i = 1; i < n+1; ++i)
        cin >> num[i];
    perfect_shuffle(num, n);
    for(int i = 1; i < n+1; ++i)
        cout << num[i] << " ";
    return 0;
}
*/

//首先介绍走圈算法，不管n是多少，下标关系总能用一个或多个圈来表示，例如1,2,3,4,5,6,7,8;可由1->2->4->8->7->5->3->1和3->6->3两个圈构成
//神级结论:引用此论文 “A Simple In-Place Algorithm for In-Shuffle” 的一个结论
//即 对于 2 * n = （3^k-1）这种长度的数组，恰好只有 k 个圈，且每个圈头部的起始位置分别是 1，3，9，...，3^(k-1)
//如果数组长度不满足上述条件，则取最接近的m,先对2m数组进行处理，再递归调用处理剩下的2(n-m)个元素
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
class Perfect_shuffle
{
    public:
        void solution(int* num, int n)//注意n是数组长度的一半而不是整个长度
        {
            while(n >= 1)
            {
                int k = 0; //记录圈的个数
                int r = 3;
                while(r-1 <= 2*n)
                {
                    ++k;
                    r *= 3;
                }
                int m = (r/3-1)/2;
                leftrotate(num+m, n-m, n);
                for(int i = 0, start = 1; i < k; ++i, start *= 3)
                {
                    cycleleader(num, start, m);
                }
                n -= m;
                num += 2*m;
            }
            
        }
    private:
        void reverse(int* num, int start, int end)
        {
            while(start < end)
            {
                swap(num[start], num[end]);
                ++start;
                --end;
            }
            return;
        }
        //用于将元素从位置n左移m
        void leftrotate(int* num, int m, int n)
        {
            reverse(num, 1, m);
            reverse(num, m+1, n);
            reverse(num, 1, n);
            return;
        }
        //转圈算法
        void cycleleader(int* num, int start, int n)
        {
            int pre = num[start];
            int mod = (2*n+1);
            int next = (2*start) % mod;
            while(next != start)
            {
                swap(pre, num[next]);
                next = (2*next) % mod;
            }
            num[start] = pre;
        }
};

int main()
{
    int a[] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
    Perfect_shuffle Solution;
    Solution.solution(a, 6);
    for(int i = 1; i <= 12; ++i)
        cout << a[i] << " ";
    return 0;
}
//拓展，如果是三个部分的数组，用同样的推导方式得到公式，详见博客
//https://www.cnblogs.com/shizhh/p/5880778.html