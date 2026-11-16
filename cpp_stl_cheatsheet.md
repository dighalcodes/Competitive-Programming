# C++ STL Quick Revision Sheet (Competitive Programming)

> Assumes C++17. Complexities: `N` = container size, `K` = elements touched/returned.

```cpp
#include <bits/stdc++.h>   // GCC only; fine for CP, not for production
using namespace std;
```

---

## 1. Vectors & Pairs

### Vector: init, sizing, access

```cpp
vector<int> v;                    // empty
vector<int> a(5);                 // {0,0,0,0,0}
vector<int> b(5, -1);             // {-1,-1,-1,-1,-1}
vector<int> c = {1, 2, 3};        // initializer list
vector<vector<int>> g(n, vector<int>(m, 0));   // n x m grid, zero-filled

v.push_back(10);                  // append, amortized O(1)
v.pop_back();                     // remove last (UB if empty!)
v.resize(8);                      // grow (new = 0) or shrink
v.resize(8, 7);                   // grow, new elements = 7
v.assign(5, 0);                   // replace all contents
v.clear();                        // size -> 0
v.reserve(100000);                // preallocate capacity, size unchanged

v.front();  v.back();  v[i];  v.at(i);   // at() bounds-checks, [] does not
v.size();   v.empty();
```

| Operation | Time | Note |
|---|---|---|
| `v[i]`, `front()`, `back()` | O(1) | `[]` out of range = UB, not an error |
| `push_back` / `pop_back` | O(1) amortized / O(1) | |
| `insert(it, x)` / `erase(it)` | O(N) | shifts elements |
| `resize`, `assign` | O(N) | |
| `clear` | O(N) | capacity is kept |

**Gotchas**
- `v.size()` is **unsigned**: `v.size() - 1` on an empty vector wraps to a huge number. Cast: `(int)v.size() - 1`.
- `push_back`/`insert` can invalidate iterators and references.
- Erase by value: `v.erase(remove(v.begin(), v.end(), x), v.end());`
- Dedup: `sort(all(v)); v.erase(unique(v.begin(), v.end()), v.end());`

```cpp
for (int x : v) {}                // read-only copy per element
for (int &x : v) x *= 2;          // modify in place
for (auto &[x, y] : vp) {}        // structured binding (C++17)
```

### Pair

```cpp
pair<int, int> p = {3, 5};
auto q = make_pair(1, "hi"s);     // pair<int, string>
p.first; p.second;
p < q;                            // compares first, then second (lexicographic)

vector<pair<int,int>> vp;
vp.push_back({1, 2});
vp.emplace_back(3, 4);            // constructs in place
auto [x, y] = p;                  // structured binding (C++17)
```

### Tuple

```cpp
tuple<int, int, string> t = {1, 2, "a"};
auto t2 = make_tuple(1, 2, 3);
get<0>(t);  get<2>(t);            // index must be compile-time constant
auto [a, b, c] = t;               // C++17
int x, y, z;
tie(x, y, z) = t2;                // unpack into existing variables
// tuples compare lexicographically too, so they work as set/map keys and in sort()
```

---

## 2. Sorting & Algorithms

### `sort()`

```cpp
sort(v.begin(), v.end());                        // ascending, O(N log N)
sort(v.begin(), v.end(), greater<int>());        // descending
sort(v.rbegin(), v.rend());                      // descending (alternative)
sort(a, a + n);                                  // plain C array
sort(v.begin() + 2, v.begin() + 7);              // sort only a subrange [2, 7)
stable_sort(v.begin(), v.end(), cmp);            // keeps order of equal elements
```

### Custom comparators

```cpp
// pairs: by second ascending
sort(vp.begin(), vp.end(), [](const pair<int,int> &a, const pair<int,int> &b) {
    return a.second < b.second;
});

// first descending, ties broken by second ascending
sort(vp.begin(), vp.end(), [](const auto &a, const auto &b) {
    if (a.first != b.first) return a.first > b.first;
    return a.second < b.second;
});

// sort indices by value
vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);                 // 0,1,2,...,n-1
sort(idx.begin(), idx.end(), [&](int i, int j) { return v[i] < v[j]; });
```

**Rule:** a comparator must be a *strict weak ordering*. Use `<` or `>`, **never** `<=` or `>=`. Returning true for equal elements can cause undefined behavior, including crashes.

### Essential `<algorithm>` tools

```cpp
reverse(v.begin(), v.end());

int mx = *max_element(v.begin(), v.end());       // returns an ITERATOR, so dereference
int mn = *min_element(v.begin(), v.end());
int pos = max_element(v.begin(), v.end()) - v.begin();   // index of the max

// Binary search: range MUST be sorted
bool found = binary_search(v.begin(), v.end(), x);       // bool only
auto lo = lower_bound(v.begin(), v.end(), x);            // first element >= x
auto hi = upper_bound(v.begin(), v.end(), x);            // first element >  x
int idxLB = lo - v.begin();                              // convert to index
int cnt = hi - lo;                                       // count of x in sorted v

// Other frequent helpers
accumulate(v.begin(), v.end(), 0LL);             // sum; use 0LL to avoid int overflow
count(v.begin(), v.end(), x);                    // O(N)
find(v.begin(), v.end(), x);                     // O(N), returns end() if missing
fill(v.begin(), v.end(), 0);
swap(a, b);
next_permutation(v.begin(), v.end());            // returns false after the last permutation
__gcd(a, b);  gcd(a, b);  lcm(a, b);             // gcd/lcm need C++17 <numeric>
```

| Function | Needs sorted? | Time |
|---|---|---|
| `sort` | n/a | O(N log N) |
| `binary_search`, `lower_bound`, `upper_bound` | **Yes** | O(log N) on vectors |
| `max_element`, `min_element`, `count`, `find` | No | O(N) |
| `reverse`, `accumulate` | No | O(N) |
| `unique` | Yes, to remove all duplicates | O(N) |

---

## 3. Sets & Multisets

### `set`: unique, sorted, balanced BST

```cpp
set<int> s;
s.insert(5);                      // ignored if already present
s.erase(5);                       // erase by value: returns number erased (0 or 1)
s.erase(s.begin());               // erase by iterator
s.count(5);                       // 0 or 1
auto it = s.find(5);              // iterator, or s.end() if absent
if (it != s.end()) { /* exists */ }

*s.begin();                       // smallest
*s.rbegin();                      // largest
s.lower_bound(x);                 // first element >= x   (O(log N))
s.upper_bound(x);                 // first element >  x
s.size();  s.empty();  s.clear();

set<int, greater<int>> sd;        // descending order
for (int x : s) {}                // iterates in sorted order
```

**Predecessor / successor trick**
```cpp
auto it = s.lower_bound(x);                      // successor candidate (>= x)
if (it != s.begin()) { auto prev = *std::prev(it); }   // largest element < x
```

### `multiset`: allows duplicates

```cpp
multiset<int> ms = {1, 1, 2, 3};
ms.insert(1);                     // now three 1s

ms.erase(1);                      // DANGER: removes ALL copies of 1
ms.erase(ms.find(1));             // SAFE: removes exactly ONE copy
                                  // (check find() != end() first; erasing end() is UB)

ms.count(1);                      // number of copies (O(log N + count))
*ms.begin();  *ms.rbegin();       // min / max
```

| Operation | `set` / `multiset` |
|---|---|
| `insert`, `erase`, `find`, `lower_bound`, `upper_bound` | O(log N) |
| `count` | O(log N) for `set`, O(log N + K) for `multiset` |
| Min / max (`begin`, `rbegin`) | O(1) |
| Iterate all | O(N) |

**Gotchas**
- Use the **member** `s.lower_bound(x)`, not `lower_bound(s.begin(), s.end(), x)`. The std version is O(N) on sets because set iterators aren't random-access.
- Set elements are const, so you cannot modify them in place. Erase and re-insert instead.
- Need a sorted set with unique elements and no ordering needed? Use `unordered_set` (same rules as `unordered_map` below).

---

## 4. Maps & Unordered Maps

### `map`: sorted keys, balanced BST, O(log N)

```cpp
map<string, int> mp;
mp["apple"] = 3;                  // insert or overwrite
mp["apple"]++;                    // fine for counting: missing int defaults to 0
mp.insert({"pear", 2});           // does NOT overwrite if the key exists
mp.erase("apple");                // by key
mp.size();

for (auto &[key, val] : mp) {}    // iterates in sorted key order
mp.begin()->first;                // smallest key
mp.rbegin()->first;               // largest key
mp.lower_bound(k);                // first key >= k
```

### `unordered_map`: hash table

```cpp
unordered_map<int, int> um;
um.reserve(1 << 18);              // fewer rehashes
um.max_load_factor(0.25);         // fewer collisions (optional)
// API is the same as map, but iteration order is arbitrary
// and there is no lower_bound / upper_bound / rbegin
```

| | `map` | `unordered_map` |
|---|---|---|
| Structure | Red-black tree | Hash table |
| Insert / find / erase | O(log N) | O(1) average, **O(N) worst case** |
| Ordered iteration | Yes | No |
| `lower_bound` / range queries | Yes | No |
| Key type | Anything with `<` | Needs a hash (`pair` / `tuple` do **not** have one by default) |

### Warning: anti-hash tests (TLE risk on Codeforces)

The default hash for integers in GCC is predictable. On Codeforces, people can craft inputs during hacks that cause huge collision chains, which turns `unordered_map` operations into O(N) and gives TLE. Two safe options:

1. Use `map` when N is around 2×10^5 or less and O(log N) is acceptable.
2. Use a randomized custom hash:

```cpp
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

unordered_map<long long, int, custom_hash> um;
unordered_set<long long, custom_hash> us;
```

### Checking existence safely

`mp[key]` **inserts** a default value (0 for `int`) if the key is missing. That changes `size()` and can silently create entries.

```cpp
// WRONG: creates the key if absent
if (mp[key] == 0) { ... }

// RIGHT
if (mp.count(key)) { ... }                       // 0 or 1
if (mp.find(key) != mp.end()) { ... }            // also gives you the value
auto it = mp.find(key);
if (it != mp.end()) cout << it->second;

mp.at(key);                                      // throws std::out_of_range if absent
```

Also: `mp[key]` does not work on a `const map`. Use `find` or `at`.

### Frequency-count pattern

```cpp
unordered_map<int,int> freq;                     // or map / custom_hash
for (int x : v) freq[x]++;
for (auto &[val, c] : freq) { /* val appears c times */ }
```

---

## 5. Adapters: `stack`, `queue`, `deque`

> `stack` and `queue` are wrappers: **no iteration, no indexing, no `clear()`**. Calling `top()`, `front()`, `back()` or `pop()` on an empty container is UB, so check `empty()` first.

### `stack`: LIFO

```cpp
stack<int> st;
st.push(1);  st.push(2);
st.top();                         // 2
st.pop();                         // removes top, returns void
st.empty();  st.size();
```

Classic uses: matching brackets, monotonic stack (next greater element), DFS.

### `queue`: FIFO

```cpp
queue<int> q;
q.push(1);  q.push(2);
q.front();                        // 1 (next to leave)
q.back();                         // 2 (most recent)
q.pop();                          // removes front, returns void
q.empty();  q.size();
```

Classic uses: BFS (`queue<pair<int,int>>` for grids).

### `deque`: double-ended, supports `[]`

```cpp
deque<int> dq;
dq.push_back(1);   dq.push_front(0);
dq.pop_back();     dq.pop_front();
dq.front();        dq.back();
dq[i];                            // O(1) random access
dq.size();  dq.empty();  dq.clear();
```

Classic uses: sliding window maximum (monotonic deque), 0-1 BFS.

| Container | Core ops | Time |
|---|---|---|
| `stack` | `push`, `pop`, `top` | O(1) |
| `queue` | `push`, `pop`, `front`, `back` | O(1) |
| `deque` | `push/pop` at both ends, `[]` | O(1) |
| `deque` insert/erase in the middle | | O(N) |

---

## 6. Priority Queues (Heaps)

```cpp
// Max-heap (default): largest on top
priority_queue<int> maxh;

// Min-heap: smallest on top
priority_queue<int, vector<int>, greater<int>> minh;

pq.push(x);                       // O(log N)
pq.top();                         // O(1), peek the extreme element
pq.pop();                         // O(log N), returns void
pq.empty();  pq.size();
```

### Common variants

```cpp
// Min-heap of pairs (Dijkstra): {distance, node}
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
pq.push({0, src});
auto [d, u] = pq.top(); pq.pop();

// Max-heap of pairs: just use the default, which compares first, then second
priority_queue<pair<int,int>> pq2;

// Trick: min-heap using negation (fine if values can't overflow when negated)
priority_queue<int> pq3;
pq3.push(-x);  int smallest = -pq3.top();

// Custom comparator (note: the logic is REVERSED vs sort)
struct Cmp {
    bool operator()(const pair<int,int> &a, const pair<int,int> &b) const {
        return a.second > b.second;          // smaller second = higher priority
    }
};
priority_queue<pair<int,int>, vector<pair<int,int>>, Cmp> pq4;
```

**Gotchas**
- With a custom comparator, `priority_queue` puts the element for which the comparator returns "less" at the *bottom*. So `>` gives a min-heap, the opposite of what many people expect from `sort`.
- No decrease-key and no iteration. For Dijkstra, push duplicate entries and skip stale ones: `if (d > dist[u]) continue;`
- Need to remove arbitrary elements? Use a `set` or `multiset` instead.
- Build a heap from a vector in O(N): `priority_queue<int> pq(v.begin(), v.end());`

---

## Quick Container Chooser

| Need | Use |
|---|---|
| Dynamic array, random access | `vector` |
| Unique + sorted + range queries | `set` |
| Sorted with duplicates, remove one at a time | `multiset` |
| Key to value, sorted keys | `map` |
| Key to value, fastest lookup | `unordered_map` (with a custom hash on Codeforces) |
| Undo or backtracking, bracket matching | `stack` |
| BFS | `queue` |
| Sliding window, 0-1 BFS | `deque` |
| Repeated min or max extraction, Dijkstra | `priority_queue` |

## Common Pitfalls Checklist

- [ ] `v.size() - 1` on an empty vector (unsigned wraparound)
- [ ] Sum overflow: use `long long` and `accumulate(..., 0LL)`
- [ ] `mp[key]` silently inserting
- [ ] `multiset::erase(value)` removing **all** copies
- [ ] `<=` in a comparator (UB)
- [ ] `binary_search` / `lower_bound` on unsorted data
- [ ] `top()` / `front()` / `pop()` on an empty container
- [ ] `unordered_map` with default hash on Codeforces
- [ ] Using `endl` in heavy output (use `'\n'`) and forgetting fast I/O:

```cpp
ios::sync_with_stdio(false);
cin.tie(nullptr);
```
