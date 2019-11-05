//关于图的关键算法及代码
#include <iostream>
#include <cstdio>
#include <bits/stdc++.h>
using namespace std;
//1.图的深度优先遍历
/*
int book[101], sum, e[101][101], n;
void dfs(int cur)
{
    //首先完成对当前节点的处理
    cout << " " << endl;
    int i = cur;
    for(i = 0; i < n; ++i)
    {
        if(book[i] == 0 && e[i][cur] == 1)
        {
            dfs(i);
            book[i] = 1;
        }
    }
    return;
}
int main()
{
    int i, j, m, a, b;
    cin >> n >> m;
    //初始化二维矩阵
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            if (i == j)e[i][j] = 0;
            else e[i][j] = 0x3f;

    //读入顶点之间的边
    for (i = 1; i <= m; i++)
    {
        cin >> a >> b;
        e[a][b] = 1;
        e[b][a] = 1; //无向图需要将其对应的点的左边也赋值为1
    }

    //从顶点1出发
    book[1] = 1;//标记一号顶点已经被访问
    dfs(1);//从1号顶点开始遍历
    system("pause");
    return 0;
}
*/
//2.图的广度优先遍历
//用队列实现，每访问一个节点，先将该节点所连的未访问的节点压入队列，再处理当前节点，最后将节点弹出
//3.最短路径算法
//注意:当边权值为1时，广度优先搜索和最短路径算法等效
void dijkstra(const vector<vector<int>>& e, vector<int>& dis, vector<bool>& vis);
int main()
{
    int n, m;//分别是点数和边数
    cin >> n >> m;
    int a, b, c;
    vector<vector<int>> e(n, vector<int>(n,0x7fffffff));
    for(int i = 0; i < m; ++i)
    {   
        cin >> a >> b >> c;
        e[a][b] = c;
    }
    vector<int> dis(n,0);
    for(int i = 0; i < n; ++i)
        dis[i] = e[0][i];
    dis[0] = 0;
    vector<bool> vis(n,false);
    vis[0] = true;
    dijkstra(e, dis, vis);
    for(int i = 0; i < n; ++i)
        cout << dis[i] << " ";
    return 0;
}
void dijkstra(const vector<vector<int>>& e, vector<int>& dis, vector<bool>& vis)
{
    int n = e.size();
    int m = e[0].size();
    int min_t = 0x7fffffff;
    for(int i = 1; i < n; ++i)
    {
        //首先找到当前最小的点
        int u;
        for(int j = 1; j < n; ++j)
        {
            if(!vis[j] && dis[j] < min_t)
            {
                min_t = dis[j];
                u = j;
            }
        }
        vis[u] = 1;
        //更新该点连接的其他点
        for(int k = 1; k < n; ++k)
        {
            if(e[u][k] != 0x7fffffff)
            {
                if(dis[u] + e[u][k] < dis[k])
                    dis[k] = dis[u]+e[u][k];
            }
        }
    }
}
//4.最小生成树算法
