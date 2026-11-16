#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

using ll = long long;

void solve()
{
    string expression;
    cin >> expression;
    stringstream ss(expression);
    vector<int> n;
    int number = 0;
    char op;

    while (ss >> number)
    {
        n.push_back(number);
        ss >> op;
    }
    sort(n.begin(), n.end());
    for (int i = 0; i <= n.size() - 1; i++)
    {
        cout << n[i];
        if (i < n.size()-1)
        {
            cout << "+";
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}