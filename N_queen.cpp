//记录N皇后问题的两种高效解法
/*
解法一:回溯法
    使用一个一维数组表示皇后的位置
    其中数组的下标表示皇后所在的行
    数组元素值表示皇后所在的列
    这样设计的棋盘，就自然避开了行冲突(这是第一个优化的地方)
*/
/*
#include <bits/stdc++.h>
using namespace std;

#define QUEEN 8 //表示棋盘大小，该值可改变
#define INITIAL -10000
int a[QUEEN];//设定一个固定长度的数组来存储元素值对应的位置
//初始化棋盘
void init()
{
    for(int i = 0; i < QUEEN; ++i)
        a[i] = INITIAL;
}
//判断某个位置是否满足放置皇后的条件
int valid(int row, int col)
{
    for(int i = 0; i < QUEEN; ++i)
    {
        if(a[i] == col || abs(i-row) == abs(a[i]-col))
            return 0;
    }
    return 1;
}
//打印一个完整的解
void print()
{
    for(int i = 0; i < QUEEN; ++i)
    {
        for(int j = 0; j < QUEEN; ++j)
        {
            if(a[i] == j)
                printf("#");
            else
                printf(".");
        }
        printf("\n");
    }
    for(int i = 0; i < QUEEN; ++i)
        printf("%d", a[i]);
    printf("\n");
	printf("--------------------------------\n");
    return;
}
//queen解法
void queen()
{
    int n = 0;
    int i = 0, j = 0;
    while(i < QUEEN)
    {
        while(j < QUEEN)
        {
            if(valid(i, j)) //如果位置(i,j)是合法的
            {
                a[i] = j;
                j = 0;
                break;
            }
            else
                ++j;
        }
        if(a[i] == INITIAL) //如果这一行中没有找到合法的列位置
        {
            if(i == 0) //表明已经访问完整个棋盘
            {
                break;
            }
            else        //表示未访问完，则回溯，访问上一行的下一列
            {
                --i;
                j = a[i]+1;
                a[i] = INITIAL;
                continue;
            }
        }
        if(i == QUEEN-1) //表示已经到最后一行，并且找到了合法的列
        {
            printf("answer %d: ", ++n);
            print();
            j = a[i] + 1;   
            a[i] = INITIAL;
            continue;
        }
        ++i;
    }
}
int main()
{
    init();
    queen();
    while(1);
    return 0;
}
*/
/*
解法二:目前为止最高效的解法(使用位运算来解决n皇后的高效解法)
核心思想是递归，row代表当前行所不能取的列，ld代表的是当前行不能取的从左向右的对角线，rd代表的是当前行不能取的从右向左的对角线
用每一位来代表该位置是否可取
*/
#include <bits/stdc++.h>
using namespace std;
long long sum = 0;
int upperlim = 1;
void test(int row, int ld, int rd)
{
    if(row != upperlim)
    {
        long pos = upperlim & ~(row|ld|rd);
        while(pos)
        {
            long p = pos & (~pos+1); //得到最靠右边的可以放置皇后的位置
            pos = pos-p;
            test(row+p, (ld+p) << 1, (rd+p) >> 1);
        }
    }
    else
        ++sum;
    return ;
}
int main()
{
    int n;
    cout << "Input the scale of the matrix: " << endl;
    cin >> n;
    time_t tm;
    tm = time(0);
    if(n <= 3 || n > 32)
    {
        cout << "规模输入错误" << endl;
        return -1;
    }
    upperlim = (upperlim << n) -1;
    test(0,0,0);
    cout << "共有" << sum << "种排列，共耗时" << (int)(time(0) - tm) << "seconds" << endl; 
}