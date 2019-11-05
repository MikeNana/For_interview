//并查集的实现，涉及知识点有:树结构，路径压缩
//常见的题目:分块、找朋友、修路等
//如果算法的运行结果超时，考虑用常规大小的数组来替换掉vector，将cin替换为scanf
#include <bits/stdc++.h>
using namespace std;
int find(vector<int>&, int);
void mix(int x, int y, vector<int>& pre);
int main()
{
    //首先初始化，确定最初有几个节点以及有几条路
    int n,k;
    cin >> n >> k;
    vector<int> pre(n,0);
    for(int i = 0; i < n; ++i)
        pre[i] = i;
    for(int i = 0; i < k; ++i)
    {
        int cmd;
        cin >> cmd;
        if(cmd == 1)
        {
            int op1, op2;
            cin >> op1 >> op2;
            if(find(pre,op1) == find(pre,op2))
                cout << "YES" << endl;
            else 
                cout << "NO" << endl;
        }
        else if(cmd == 2)
        {
            int op1,op2;
            cin >> op1 >> op2;
            mix(op1,op2,pre);
            continue;
        }
    }
    return 0;
}

void mix(int x, int y, vector<int>& pre)
{
    if(find(pre,x) != find(pre,y))
        pre[x] = y;
    return;
}

int find(vector<int>& pre, int x)
{
    //首先向上找到根节点
    int r = x;
    while(r != pre[r])
        r = pre[r];
    //开始路径压缩，为了接近O(1)的时间复杂度，j用作临时变量
    int i = x,j;
    while(pre[i] != r)
    {
        j = pre[i];
        pre[i] = r;
        i = j;
    }
    return r;
}

int find1(vector<int>&pre, int x)
{
    return x == pre[x] ? x : pre[x] = find1(pre, pre[x]);
}