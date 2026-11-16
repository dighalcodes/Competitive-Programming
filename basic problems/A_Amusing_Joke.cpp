#include <bits/stdc++.h>
using namespace std;
#include <string>
#include <set>

using ll = long long;

void solve()
{
    string host;
    cin >> host;

    string guest;
    cin >> guest;

    multiset<char> ms(host.begin(), host.end());
    ms.insert(guest.begin(), guest.end());

    string pile;
    cin >> pile;

    multiset<char> ms_pile(pile.begin(), pile.end());

    if(ms==ms_pile){
        cout << "YES";
    }else{
        cout << "NO";
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}