#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

//交换两个整数
void swap(int &a, int &b)
{
    int temp = 0;
    temp = a;
    a = b;
    b = temp;
}
//1.冒泡排序法:时间复杂度是O(n平方)，是稳定算法，因为是相邻的元素进行比较
void bubble_sort(vector<int> &num)
{
    unsigned n = num.size();
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n-i-1; ++j)
            if(num[j] > num[j+1])
                swap(num[j],num[j+1]);
    return;
}

//2.插入排序法，时间复杂度为O(n平方)，是稳定的
void insert_sort(vector<int> &num)
{
    int n = num.size();
    for(int i = 0; i < n; ++i)
    {
        int j = i;
        while(j >= 0)
        {
            if(num[j] < num[j-1])
            {
                swap(num[j], num[j-1]);
                --j;
            }
            else 
                break;
        }
    }
    return;
}

//3.选择排序法，时间复杂度为O(n平方)，不是一个稳定的算法，例如{7,7,2}按升序排
void choose_sort(vector<int> &num)
{
    int n = num.size();
    for(int i = 0; i < n; ++i)
    {
        int temp = num[i];
        int j = 0;
        int index = i;
        for(j = i+1; j < n; ++j)
        {   
            if(num[j] < temp)
            {
                index = j;
                temp = num[j];
            }       
        }
        if(temp != num[i])
            swap(num[i], num[index]);
    }
    return ;
}

//4.快速排序法:平均时间复杂度为O(nlogn)，算法不稳定，因为元素有跳跃，因为有递归栈，所以空间复杂度为O(nlogn)
//非稳定的例子{5,3,3,4,3,6}按升序
void quick_sort1(vector<int> &num, int left, int right)
{
    if(left >= right)
        return;
    int i = left, j = right;
    int key = num[i];
    while(i < j)
    {
        while((i < j) && (key <= num[j]))
            --j;
        num[i] = num[j];
        while((i < j) && (key >= num[i]))
            ++i;
        num[j] = num[i];
    }
    num[i] = key;
    quick_sort1(num, left, i-1);
    quick_sort1(num, i+1, right);
    return;
}

//5. 堆排序法，需要先构造堆，再进行排序，时间复杂度为O(nlogn)，算法并不稳定
void Heap_adjust(vector<int>& num, int i, int n)
{
    int child;
    child = 2*i+1;
    int tmp;
    for(; 2*i+1 < n; i = child)
    {
        child = 2*i+1;
        //选出较大的子节点
        if(child < n-1 && num[child] < num[child+1])
            ++child;
        //如果该子节点大于父节点，则交换两个节点
        if(num[i] < num[child])
        {
            tmp = num[i];
            num[i] = num[child];
            num[child] = tmp;
        }
        else 
            break;
    }
    return;
}

void Heap_sort(vector<int>& num, int n)
{
    int i;
    //首先对所有非叶节点进行调整
    for(i = (n-1)/2; i >= 0; --i)
        Heap_adjust(num, i, n);
    //开始排序
    for(i = n-1; i > 0; --i)
    {
        num[i] = num[0] ^ num[i];
        num[0] = num[0] ^ num[i];
        num[i] = num[0] ^ num[i];
        Heap_adjust(num, 0, i);
    }
    return;
}

//6.归并排序，利用了分治的思想，利用递归将所有元素重新排序，时间复杂度为O(nlogn)，是具有稳定性的，空间复杂度为O(n)
void merge(vector<int> &num, int left, int mid, int right, vector<int> temp, int& res)
{
    int i = left, j = mid+1, k = 0;
    while(i <= mid && j <= right)
    {
        if(num[i] <= num[j])
        {
            temp[k++] = num[i++];
            res += (right-j+1)*num[i];
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
    return;
}
void merge_sort(vector<int> &num, int left, int right, vector<int> temp, int& res)
{
    if(left < right)
    {
        int mid = (left + right) >> 1;
        merge_sort(num, left, mid, temp, res);
        merge_sort(num, mid+1, right, temp, res);
        merge(num, left, mid, right, temp, res);
    }
    return;
}

//7.基数排序是稳定算法，时间复杂度为O(d(r+n))，空间复杂度为O(r)，首先获得最大数的位数，其次从最低位向
//最高位遍历，每次遍历更新数组，最后得到排序结果
int get_index(vector<int> num)
{
    int n = num.size();
    if(n <= 0)
        return 0;
    int maxnum = num[0];
    for(int i = 0; i < n; ++i)
    {
        if(maxnum < num[i])
            maxnum = num[i];
    }
    int res = 1;
    while(maxnum / 10 != 0)
    {
        ++res;
        maxnum /= 10;
    }
    return res;
}
//获取数字num的第d位数是多少
int get_pos(int num, int d)
{
    int index = pow(10, d);
    num %= index;
    return num / pow(10, d-1);
}
//进行基数排序
void radix_sort(vector<int> &num)
{
    int n = num.size();
    vector<vector<int>> bucket(10);
    //将每个元素按第d位数放置在对应的桶中
    for(int d = 1; d <= get_index(num); ++d)
    {
        for(int i = 0; i < num.size(); ++i)
        {
            bucket[get_pos(num[i],d)].push_back(num[i]);
        }
        //根据桶中元素将num重新填满
        int k = 0;
        for(int i = 0; i <= 9; ++i)
        {
            int j = 0;
            while(j < bucket[i].size())
                num[k++] = bucket[i][j++];
            bucket[i].clear();
        }
    }
}
int main()
{
    vector<int> num{71,11,82,32,44,13,17,95,54,28,79,56};
    int res = 0;
    int n = num.size();
    vector<int> temp(n);
    merge_sort(num, 0, n-1, temp, res);
    for(auto a:num)
        cout << a << " ";
    cout << res << endl;
    return 0;
}