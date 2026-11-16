#include <bits/stdc++.h>
using namespace std;
#include <iostream>
#include <string>
#include <algorithm>
using ll = long long;

void solve()
{
    int n, t;
    cin >> n >> t;
    string s;
    cin >> s;
    for (int sec = 0; sec < t; sec++)
    {
        for (int i = 1; i < n; i++)
        {
            if (s[i - 1] == 'B' && s[i] == 'G')
            {
                char temp = s[i - 1];
                s[i - 1] = s[i];
                s[i] = temp;
                i++;
            }
        }
    }
    cout << s;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}