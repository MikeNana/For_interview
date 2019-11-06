//记录马拉车算法，该算法用于解决最长回文字符串问题，而且好像只能用于最长回文字符串问题
//1.首先将对字符串作预处理，避开讨论奇偶的问题
//2.需要维护的是一个半径数组
//3.不断更新最长回文字符串的中心位置和半径
#include <vector>
#include <iostream>

using namespace std;
int manacher(const string& str)
{
    //字符串预处理
    string temp = "";
    temp += '#';
    for(int i = 0; i < str.size(); ++i)
    {
        temp += str[i];
        temp += '#';
    }
    int n = temp.size();
    //维护半径数组
    vector<int> p(n, 0);
    int c = -1, right = -1;
    int ans = 0;
    for(int i = 0; i < n; ++i)
    {
        p[i] = i < right ? min(right-i, p[2*c-i]) : 1;
        while(i + p[i] <= n-1 && i - p[i] >= 0 && temp[i + p[i]] == temp[i - p[i]])
            ++p[i];
        if(i+p[i] > right)
        {
            c = i;
            right = i+p[i];
        }
        ans = max(ans, p[i]);
    }
    return ans-1;
}
int main()
{
    while(1)
    {
        string test;
        cin >> test;
        cout << manacher(test) << endl;
    }
    return 0;
}