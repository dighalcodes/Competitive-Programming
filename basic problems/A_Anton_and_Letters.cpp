#include <bits/stdc++.h>
using namespace std;
#include <set>
using ll = long long;

void solve()
{
    string line;
    getline(cin, line);
    set<char> s;
    for (char c : line)
    {
        if (isalpha(c))
        {
            s.insert(c);
        }
    }
    cout << s.size();
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}