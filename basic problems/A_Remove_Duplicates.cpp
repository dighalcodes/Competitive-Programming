#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve()
{
    int n;
    cin >> n;

    vector<int> a(n);
    int last[1001] = {};

    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        last[a[i]] = i;
    }

    vector<int> res;
    for (int i = 0; i < n; i++)
    {
        if (last[a[i]] == i)
        {
            res.push_back(a[i]);
        }
    }

    cout << res.size() << "\n";
    for (int x : res)
    {
        cout << x << " ";
    }
    cout << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}