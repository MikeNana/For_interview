#include <iostream>
#include <vector>

using namespace std;
const int process_num = 50;     //最大进程数
const int resources_num = 50;   //最大资源数
class Banker
{
private:
    int m, n; //进程数量，资源种类
    vector<vector<int> > max_;      //每个进程所需每个资源的最大数量
    vector<vector<int>> allocation; //已经分配资源的数量
    vector<vector<int>> need;       //每个进程还需要的资源数量
    vector<int> avaliable;          //目前可用的资源
    vector<vector<int>> request;    //进程申请的资源数
    vector<bool> finish;            //当前进程是否可被分配资源
    vector<int> p;                  //记录安全序列 
public:
    Banker():max_(process_num, vector<int>(resources_num,0)),
            allocation(process_num, vector<int>(resources_num,0)),
            need(process_num, vector<int>(resources_num,0)),
            avaliable(process_num, 0),
            request(process_num, vector<int>(resources_num,0)),
            finish(process_num, false),
            p(process_num,0){}
    void init();
    void display();
    bool safe_detect();
    void banker();
};

void Banker::init()
{
    cout << "请输入进程数目和资源种类数目......\n";
    cin >> m >> n;
    cout << "请输入每个进程所需每种资源的数目，按照" << m << "*" << n << "的格式输入." << endl;
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; ++j)
            cin >> max_[i][j];
    }
    cout << "请输入每个进程已经分配的每种资源的数目，按照" << m << "*" << n << "的格式输入." << endl;
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            cin >> allocation[i][j];
            need[i][j] = max_[i][j] - allocation[i][j];
        }
    }
    cout << "请输入每种资源可用的数目." << endl;
    for(int i = 0; i < n; ++i)
        cin >> avaliable[i];
    
}
void Banker::display()
{
    cout << "当前每种资源的可用数目为: " << endl;
    for(int i = 0; i < n; ++i)
        cout << avaliable[i] << "       ";
    cout << endl;
    cout << "每个进程需要的每种资源数目为: " << endl;
    for(int i = 0; i < m; ++i)
    {
        cout << "进程" << i << "还需要:  ";
        for(int j = 0; j < n; ++j)
            cout << need[i][j] << "     ";
        cout << endl;
    }
    cout << "每个进程已经分配的每种资源数目为: " << endl;
    for(int i = 0; i < m; ++i)
    {
        cout << "进程" << i << "已分配:  ";
        for(int j = 0; j < n; ++j)
            cout << allocation[i][j] << "     ";
        cout << endl;
    }

    return;
}
//银行家算法，
void Banker::banker()
{
    int cusneed, cnt = 0;
    bool valid = true;
    char again;
    while(1)
    {
        display();
        cout << endl;
        //首先判断输入的合法性
        while(true)
        {
            valid = true;
            cout << "请输入要申请资源的进程号:  " << endl;
            cin >> cusneed;
            if(cusneed > m)
            {
                cout << "没有此进程，请重新输入." << endl;
                continue;
            }
            cout << "请输入要申请的每种资源的数目: " << endl;
            for(int i = 0; i < n; ++i)
            {
                cin >> request[cusneed][i];
                if(request[cusneed][i] > need[cusneed][i])
                {
                    cout << "要申请的资源数目超过需要的数目." << endl;
                    valid = false;
                }
                if(request[cusneed][i] > avaliable[i])
                {
                    cout << "要申请的资源数目超过可用的数目." << endl;
                    valid = false;
                }
            }
            if(valid)
                break;
        }
        //在输入合法的情况下，采用银行家算法
        //首先试分配资源给申请资源的进程
        for(int i = 0; i < n; ++i)
        {
            avaliable[i] -= request[cusneed][i];
            allocation[cusneed][i] += request[cusneed][i];
            need[cusneed][i] -= request[cusneed][i];
        }
        //进行安全判断，如果安全则分配，否则收回资源
        if(safe_detect())
            cout << "同意分配资源" << endl;
        else
        {
            cout << "请求被拒绝" << endl;
            for(int i = 0; i < n; ++i)
            {
                avaliable[i] += request[cusneed][i];
                allocation[cusneed][i] -= request[cusneed][i];
                need[cusneed][i] += request[cusneed][i];
            }
        }
        for(int i = 0; i < n; ++i)
            if(need[cusneed][i] <= 0)
                ++cnt;
        if(cnt == n)    //表示该进程的所有要求都被满足，所有资源都可被回收
        {
            for(int i = 0; i < n; ++i)
            {
                need[cusneed][i] = 0;
                avaliable[i] += allocation[cusneed][i];
                allocation[cusneed][i] = 0;
            }
            cout << "进程" << cusneed << "占有的资源已经被释放了" << endl;
            cnt = 0;
        }
        for(int i = 0; i < m; ++i)
            finish[i] = false;
        cout << "你还想再次请求分配吗？是请按Y/y，否请按其他键！" << endl;
        cin >> again;
        if (again == 'Y' || again == 'y')
            continue;
        break;
    }
}
/*
bool Banker::safe_detect()
{
    int l = 0, j = 0;
    int work[n];
    for(int i = 0; i < n; ++i)
        work[i] = avaliable[i];
    for(int i = 0; i < m; ++i)
        finish[i] = false;
    for(int i = 0; i < m; ++i)
    {
        if(finish[i] == true)
            continue;
        else
        {
            for(j = 0; j < n; ++j)
            {
                if(need[i][j] > work[j])
                    break;
            }
            if(j == n)
            {
                finish[i] = true;
                for(int k = 0; k < n; ++k)
                    work[k] += allocation[i][k];
                p[l++] = i;
                i = -1;
            }
            else 
                continue;
        }
    }

    if(l == m)
    {
        cout << "系统是安全的" << endl;
        cout << "安全序列为" << endl;
        for(int i = 0; i < m; ++i)
        {
            cout << p[i];
            if(i != m-1)
                cout << "--->";
        }
        cout << endl;
        return true;
    }
    return false;
}
*/
bool Banker::safe_detect()
{
    int j = 0, l = 0;
    int work[n];
    for(int i = 0; i < n; ++i)
        work[i] = avaliable[i];
    for(int i = 0; i < m; ++i)
        finish[i] = false;
    for(int i = 0; i < m; ++i)
    {
        if(finish[i] == true)
            continue;
        else
        {
            for(j = 0; j < n; ++j)
            {
                if(need[i][j] > work[j])
                    break;
            }
            if(j == n)
            {
                finish[i] = true;
                p[l++] = i;
                for(int k = 0; k < n; ++k)
                    work[k] += allocation[i][k];
                i = -1;
            }
            else
                continue;
        }
    }
    if(l == m)
    {
        cout << "系统是安全的" << endl;
        cout << "安全序列如下: " << endl;
        for(int i = 0; i < m; ++i)
        {
            cout << p[i];
            if(i != m-1)
                cout << "---->";
        }
        cout << endl << endl;
        return true;
    }
    return false;
}
int main()
{
    Banker mike;
    mike.init();
    mike.safe_detect();
    mike.banker();
    return 0;
}