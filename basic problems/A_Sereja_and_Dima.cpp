#include <bits/stdc++.h>
using namespace std;
#include <deque>
using ll = long long;

void solve()
{
    int n;
    cin >> n;
    deque<int> dq;
    int turn = 0, sumS = 0, sumD = 0;
    for (int i = 0; i < n; i++)
    {
        int x;
        cin>>x;
        dq.push_back(x);
    }
    while (!dq.empty())
    {
        if (dq.front() > dq.back())
        {
            if (turn % 2 == 0)
            {
                sumS += dq.front();
            }
            else
            {
                sumD += dq.front();
            }
            dq.pop_front();
        }
        else
        {
            if (turn % 2 == 0)
            {
                sumS += dq.back();
            }
            else
            {
                sumD += dq.back();
            }
            dq.pop_back();
        }
        turn++;
    }
    cout << sumS << " " << sumD;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}