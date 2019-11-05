#include <vector>
#include <iostream>
#include <assert.h>
#include <stack>
#include <algorithm>
#include <deque>
#include <iomanip>
using namespace std;


//1.计算数组的小和
long long smallsum(vector<int> &num, int left, int mid, int right)
{
    long long res = 0;
    assert(left <= mid);
    assert(mid <= right);
    vector<int>temp(10000);
    int i = left, j = mid + 1, k = 0;
    while(i <= mid && j <= right)
    {
        if(num[i] < num[j])
        {
            res += (right-j+1)*num[i];
            temp[k++] = num[i++];
        }
        else
            temp[k++] = num[j++];
    }
    while(i <= mid)
        temp[k++] = num[i++];
    while(j <= right)
        temp[k++] = num[j++];
    k = 0;
    while(left <= right)
        num[left++] = temp[k++];
    return res;
}
long long Smallsum(vector<int>& num, int left, int right)
{
    if(left >= right)
        return 0;
    int mid = (right+left) >> 1;
    return Smallsum(num, left, mid) + Smallsum(num, mid+1, right) + smallsum(num, left, mid, right);
}

//2.计算最长递增子序列(3种解法)
//将问题转化为最长公共子序列
int LIS_helper(vector<int> num, vector<int>& ans)
{
    vector<int> temp = num;
    sort(num.begin(), num.end());
    int n = num.size();
    vector<vector<int>> dp(n+1, vector<int>(n+1,0));
    for(int i = 1; i <= n; ++i)
    {
        for(int j = 1; j <= n; ++j)
        {
            if(num[i-1] == temp[j-1])
            {
                int tmp = num[i-1];
                ans.push_back(tmp);
                dp[i][j] = 1+dp[i-1][j-1];
            }
            else
            {
                int tmp = max(dp[i-1][j], dp[i][j-1]);
                dp[i][j] = max(dp[i-1][j-1], tmp);
            }
        }
    }
    return dp[n][n];
}
//3.计算最长对称子字符串
string longestPalindrome(string s) {
        string temp;
        temp += "#";
        for(int i = 0; i < s.size(); ++i)
        {
            temp += s[i];
            temp += "#";
        }
        int n = temp.size();
        int res = 0;
        string ans;
        cout << temp << endl;
        for(int i = 1; i < n; ++i)
        {
            int j = i-1, k = i+1;
            int count = 1;
            while(j > 0 && k < n && temp[j] == temp[k])
            {
                count += 2;
                --j;
                ++k;
            }
            if(count > res)
            {
                ans = temp.substr(j+1, k-j-1);
                res = count;
            }
        }
        for(int i = 0; i < ans.size(); ++i)
        {
            if(ans[i] == '#')
                ans.erase(ans.begin()+i);
        }
        return ans;
    }

//4.约瑟夫环问题的最佳解法:循环队列
//该问题也可以用循环链表来做，但是由于没有现成的数据结构，有点麻烦
void Joseph(deque<int>& dq, int m)
{
    while(dq.size() > 1)
    {
        int temp = 1;
        while(temp < m)
        {
            int tmp = dq.front();
            dq.pop_front();
            dq.push_back(tmp);
            ++temp;
        }
        dq.pop_front();
    }
    cout << dq.back() << endl;
    return;
}

//5.利用二分法求最长递增子序列
void LIS(vector<int> num)
{
    int n = num.size();
    //用于动态规划，同时还用了二分法
    vector<int> dp(n);
    //用于记录元素的位置
    vector<int> pos(n);
    int k = 0;
    //利用动态规划和二分法得到dp和pos，这里决定了该算法的时间复杂度为O(nlogn)
    for(int i = 0;i<n;i++){
        if(k==0||dp[k-1]<num[i]){
            pos[i] = k;
            dp[k++] = num[i];
        }else{
            auto it = lower_bound(dp.begin(), dp.begin()+k, num[i]);
            *it = num[i];
            pos[i] = it-dp.begin();
        }
    }
    vector<int> res(k,1e8);
    //visit用于记录是否访问过正确答案中的第i个元素
    vector<bool> visited(k+1,false);
    visited[k] = true;
    int val = k-1;
    for(int i = n-1;i>=0;i--){
        if(pos[i]==val&&visited[val+1]){
            res[val] = min(res[val],num[i]);
            visited[val]=true;
            val--;
        }
    }
    for(int i = 0;i<k;i++)
        cout<<res[i]<<" ";
}

//6.信封嵌套问题
bool cmp(const pair<int,int>& a, const pair<int, int>& b)
{
    return a.first < b.first;
}
int envelope(vector<pair<int,int>>& env)
{
    //首先按长度对所有pair进行排序
    sort(env.begin(), env.end(), cmp);
    //分组存储对应的宽度
    vector<vector<int>> w;
    int index = env[0].first;
    for(int i = 0; i < env.size();)
    {
        vector<int> temp;
        while(i < env.size() && env[i].first == index)
        {
            temp.push_back(env[i].second);
            ++i;
        }
        w.push_back(temp);
        if(i < env.size())
            index = env[i].first;
    }
    /*
    for(int i = 0; i < w.size(); ++i)
    {
        for(int j = 0; j < w[i].size(); ++j)
            cout << w[i][j] << " ";
        cout << endl;
    }
    */
    //对每个长度对应的vector中的元素进行排序
    for(int i = 0; i < w.size(); ++i)
    {
        sort(w[i].begin(), w[i].end());
        reverse(w[i].begin(), w[i].end());
    }
    //用dp来存储每个长度对应的最后的元素
    vector<int> dp;
    //用二分法找出最大递增序列
    dp.push_back(0);
    int k = 0;
    for(int i = 0; i < w.size(); ++i)
    {
        for(int j = 0; j < w[i].size(); ++j)
        {
            if(j == 0 && w[i][j] > dp[k])
            {
                dp.push_back(w[i][j]);
                ++k;
                continue;
            }
            else
            {
                auto it = lower_bound(dp.begin(), dp.begin()+k, w[i][j]);
                *it = w[i][j];
            }
        }
    }
    return k;
}

//7.汉诺塔问题的递归解法
template<typename T> 
void hanoi(int n, T src, T des, T tmp)
{
    if(n > 0)
    {
        hanoi(n-1, src, tmp, des);
        cout << src << " -> " << des << endl;
        hanoi(n-1, tmp, des, src);
    }
    return;
}

//8.用栈实现四则运算
//(1)首先将中缀表达式转为后缀表达式
//(2)对后缀表达式进行计算
string change(const string s)
{
    string res;
    stack<char> s1;
    int k = 0;
    while(k < s.size())
    {
        if(isdigit(s[k]))
        {
            int left = k;
            while(isdigit(s[left]))
                ++left;
            res += s.substr(k, left-k);
            res += " ";
            k = left;
            continue;
        }
        if(s[k] == '*' || s[k] == '/')
        {
            s1.push(s[k++]);
            continue;
        }
        if(s[k] == '+')
        {
            while(!s1.empty() && (s1.top() == '*' || s1.top() == '/' || s1.top() == '-'))
            {
                res += s1.top();
                res += " ";
                s1.pop();
            }
            s1.push(s[k++]);
            continue;
        }
        if(s[k] == '-')
        {
            while(!s1.empty() && (s1.top() == '*' || s1.top() == '/' || s1.top() == '+'))
            {
                res += s1.top();
                res += " ";
                s1.pop();
            }
            s1.push(s[k++]);
            continue;
        }
        if(s[k] == '(')
        {
            s1.push(s[k++]);
            continue;
        }
        if(s[k] == ')')
        {
            while(!s1.empty() && s1.top() != '(')
            {
                res += s1.top();
                res += " ";
                s1.pop();
            }
            s1.pop();
            ++k;
            continue;
        }
    }
    while(!s1.empty())
    {
        res += s1.top();
        res += " ";
        s1.pop();
    }
    return res;
}
int calculate(const string& s)
{
    stack<int> res;
    int k = 0;
    while(k < s.size())
    {
        if(s[k] == ' ')
        {
            ++k;
            continue;
        }
        if(isdigit(s[k]))
        {
            int left = k;
            while(isdigit(s[left]))
                ++left;
            string tmp = s.substr(k, left-k);
            res.push(stoi(tmp));
            k = left;
            continue;
        }
        switch(s[k])
        {
            case '+':
            {
                int index1 = res.top();
                res.pop();
                int index2 = res.top();
                res.pop();
                int temp = index1+index2;
                res.push(temp);
                ++k;
                break;
            }
            case '-':
            {
                int index1 = res.top();
                res.pop();
                int index2 = res.top();
                res.pop();
                int temp = index2-index1;
                res.push(temp);
                ++k;
                break;
            }
            case '*':
            {
                int index1 = res.top();
                res.pop();
                int index2 = res.top();
                res.pop();
                int temp = index1*index2;
                res.push(temp);
                ++k;
                break;
            }
            case '/':
            {
                int index1 = res.top();
                res.pop();
                int index2 = res.top();
                res.pop();
                int temp = index2/index1;
                res.push(temp);
                ++k;
                break;
            }
        }
    }
    return  res.top();
}

//9.KMP算法(无回溯的模式匹配，kmp是三个人名的缩写)
//主要思想是:利用已得到的部分匹配信息来进行后面的匹配过程，消除主串的回溯，从而提高匹配效率
void get_next(const string& a, vector<int>& next)
{
    next[0] = -1;
    int first = 0, last = -1;
    while(first < a.size()-1)
    {
        if(last == -1 || a[last] == a[first])
        {
            ++first;
            ++last;
            if(a[first] == a[last])
                next[first] = next[last];
            else
                next[first] = last;
        }
        else
            last = next[last];
    }
    return;
}
string KMP(const string& s, const string& t, vector<int>& next)
{
    int a = 0, b = 0;
    while(a < s.size())
    {
        while(b < t.size() && s[a] == t[b])
        {
            ++a;
            ++b;
        }
        if(b == t.size())
            return s.substr(a-b,b);
        else
        {
            if(b == -1)
            {
                ++a;
                ++b;
                continue;
            }
            else
                b = next[b];
        }
    }
    return "end";
}

//10.最小编辑距离
int helper(const string& str1, const string& str2, int ic, int dc, int rc)
{
    int n = str1.size(), m = str2.size();
    vector<vector<int>> dp(n+1, vector<int>(m+1,0));
    for(int i = 1; i <= m; ++i)
        dp[0][i] = i*ic;
    for(int j = 1; j <= n; ++j)
        dp[j][0] = j*dc;
    for(int i = 1; i <= n; ++i)
    {
        for(int j = 1; j <= m; ++j)
        {
            if(str1[i-1] == str2[j-1])
            {
                int tmp = min(dp[i-1][j],dp[i][j-1]);
                dp[i][j] = min(dp[i-1][j-1], tmp);
            }
            else
            {
                int tmp = min(rc+dp[i-1][j-1], dc+dp[i-1][j]);
                dp[i][j] = min(tmp, ic+dp[i][j-1]);
            }
        }
    }
    return dp[n][m];
}

//11.表达式得到期望结果的组合种数，可能的运算符有'|', '&', '^','0','1'
bool valid(const string& exp)
{
    int n = exp.size();
    for(int i = 0; i < n; ++i)
    {
        if(i % 2 == 0)
        {
            if(exp[i] != '1' && exp[i] != '0')
                return false;
        }
        else
            if(exp[i] != '|' && exp[i] != '&' && exp[i] != '^')
                return false;
            
    }
    return true;
}
int helper(const string& exp, const string& desire)
{
    int n = exp.size();
    if(n <= 0 || n % 2 == 0 || !valid(exp))
        return 0;
    vector<vector<long long>> t(n, vector<long long>(n,0)), f(n, vector<long long>(n,0));
    for(int i = 0; i < n; ++i)
    {
        t[i][i] = exp[i] == '1' ? 1 : 0;
        f[i][i] = exp[i] == '1' ? 0 : 1;
    }
    for(int i = 0; i < n; i += 2)
    {
        for(int j = i-2; j >= 0; j -= 2)
        {
            for(int k = j; k < i; k += 2)
            {
                if(exp[k+1] == '|')
                {
                    
                    t[j][i] += t[j][k]*f[k+2][i]+t[j][k]*t[k+2][i]+f[j][k]*t[k+2][i];
                    t[j][i] %= 1000000007;
                    f[j][i] += f[j][k]*f[k+2][i];
                    f[j][i] %= 1000000007;
                }
                else if(exp[k+1] == '&') 
                {
                    t[j][i] += t[j][k]*t[k+2][i];
                    t[j][i] %= 1000000007;
                    f[j][i] += t[j][k]*f[k+2][i]+f[j][k]*f[k+2][i]+f[j][k]*t[k+2][i];
                    f[j][i] %= 1000000007;
                }
                else if(exp[k+1] == '^')
                {
                    t[j][i] += t[j][k]*f[k+2][i]+f[j][k]*t[k+2][i];
                    t[j][i] %= 1000000007;
                    f[j][i] += f[j][k]*f[k+2][i]+t[j][k]*t[k+2][i];
                    f[j][i] %= 1000000007;
                }
            }
        }
    }
    return desire == "false" ? f[0][n-1] : t[0][n-1];
}
int main()
{
    string exp, desire;
    cin >> exp >> desire;
    cout << helper(exp, desire) << endl;
    return 0;
}
