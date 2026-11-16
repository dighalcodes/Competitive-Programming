# Java Collections Framework (JCF) Quick Revision Sheet

> Targets Java 8+, with Java 9/10 features marked where used. `N` = number of elements.
> Imports: `import java.util.*;` (and `java.util.concurrent.*` for `ConcurrentHashMap`).

---

## 1. Hierarchy & Core Interfaces Overview

```
Iterable
  └── Collection
        ├── List ............ ArrayList, LinkedList, (Vector, Stack: legacy)
        ├── Set
        │     ├── HashSet
        │     │     └── LinkedHashSet
        │     └── SortedSet → NavigableSet ... TreeSet
        └── Queue
              ├── PriorityQueue
              └── Deque ....... ArrayDeque, LinkedList

Map  (NOT a Collection)
  ├── HashMap
  │     └── LinkedHashMap
  ├── SortedMap → NavigableMap ... TreeMap
  └── ConcurrentHashMap (via ConcurrentMap)
```

### Core interfaces at a glance

| Interface | Ordered? | Duplicates? | Main implementations | Use it when |
|---|---|---|---|---|
| `List` | Yes (index) | Yes | `ArrayList`, `LinkedList` | Ordered sequence, index access |
| `Set` | Depends on impl | **No** | `HashSet`, `LinkedHashSet`, `TreeSet` | Uniqueness / membership checks |
| `Map` | Depends on impl | Keys unique, values can repeat | `HashMap`, `LinkedHashMap`, `TreeMap`, `ConcurrentHashMap` | Key to value lookup |
| `Queue` | FIFO (or priority) | Yes | `ArrayDeque`, `LinkedList`, `PriorityQueue` | BFS, scheduling, top-K |
| `Deque` | Both ends | Yes | `ArrayDeque`, `LinkedList` | Stack **and** queue in one class |

### Time complexity summary

| Collection | Add | Remove | Get / Contains | Notes |
|---|---|---|---|---|
| `ArrayList` | O(1) amortized (end); O(N) at index | O(N) (O(1) at end) | `get(i)` O(1); `contains` O(N) | Contiguous array |
| `LinkedList` | O(1) at ends | O(1) at ends; O(N) to find the middle | `get(i)` O(N); `contains` O(N) | Doubly linked |
| `HashSet` / `HashMap` | O(1) avg | O(1) avg | O(1) avg | Worst case O(log N) in Java 8+ (treeified bins), assuming reasonable keys |
| `LinkedHashSet` / `LinkedHashMap` | O(1) avg | O(1) avg | O(1) avg | Same as hash, plus order |
| `TreeSet` / `TreeMap` | O(log N) | O(log N) | O(log N) | Sorted; `first`/`last` also O(log N) |
| `ArrayDeque` | O(1) at both ends | O(1) at both ends | `peek` O(1); `contains` O(N) | No index access |
| `PriorityQueue` | O(log N) | `poll` O(log N); `remove(obj)` O(N) | `peek` O(1); `contains` O(N) | Binary heap |

---

## 2. Lists

### `ArrayList`: dynamic array

```java
List<Integer> list = new ArrayList<>();
List<Integer> init = new ArrayList<>(Arrays.asList(3, 1, 2));
List<Integer> sized = new ArrayList<>(1000);     // pre-size when you know N

list.add(10);                // append
list.add(0, 5);              // insert at index (O(N), shifts)
list.get(0);                 // O(1)
list.set(0, 99);             // overwrite, returns old value
list.remove(0);              // remove by INDEX
list.remove(Integer.valueOf(99));   // remove by VALUE
list.size();  list.isEmpty();  list.contains(5);  list.indexOf(5);
list.clear();
```

- **Internals:** backed by an `Object[]`. When full, it allocates a bigger array (about 1.5x in the OpenJDK implementation, but don't quote that as a spec) and copies.
- **Default capacity:** 10, allocated lazily on the first add.
- **Use when:** you need fast random access or mostly append and iterate. It is the default choice.

### `LinkedList`: doubly linked list

```java
LinkedList<Integer> ll = new LinkedList<>();
ll.addFirst(1);   ll.addLast(2);
ll.removeFirst(); ll.removeLast();
ll.getFirst();    ll.getLast();
// Implements both List and Deque
```

- **Internals:** each node holds `prev`, `next`, and the value, so there is extra memory per element and poor cache locality.
- **Insert/delete is O(1) only if you already hold the node or iterator position.** Reaching index `i` is still O(N).

### `ArrayList` vs `LinkedList`

| | `ArrayList` | `LinkedList` |
|---|---|---|
| `get(i)` | **O(1)** | O(N) |
| Add/remove at end | O(1) amortized | O(1) |
| Add/remove at front | O(N) | **O(1)** |
| Add/remove in middle | O(N) | O(N) to reach + O(1) to link |
| Memory | Compact | Heavier (node objects) |
| Typical verdict | **Default choice** | Rarely better in practice |

> In interviews, say: "I'd use `ArrayList` by default. For queue or stack behavior I'd pick `ArrayDeque` over `LinkedList`." Benchmarks usually favor `ArrayList` even for many mid-list inserts, because of cache effects, but this depends on the workload, so measure if it matters.

### List traps

```java
List<Integer> a = new ArrayList<>(List.of(10, 20, 30));
a.remove(1);                     // removes INDEX 1 -> [10, 30]
a.remove(Integer.valueOf(10));   // removes VALUE 10 -> [30]

// ConcurrentModificationException: never modify a list inside a for-each over it
for (Integer x : a) { if (x == 30) a.remove(x); }    // BAD

a.removeIf(x -> x == 30);                            // GOOD
Iterator<Integer> it = a.iterator();                 // GOOD
while (it.hasNext()) if (it.next() == 30) it.remove();
```

---

## 3. Sets

| Implementation | Order | Time | Null allowed? | Backed by |
|---|---|---|---|---|
| `HashSet` | None (don't rely on it) | O(1) avg | One `null` | `HashMap` |
| `LinkedHashSet` | Insertion order | O(1) avg | One `null` | `LinkedHashMap` |
| `TreeSet` | Sorted | O(log N) | **No** (with natural ordering) | `TreeMap` |

```java
Set<Integer> hs = new HashSet<>();
hs.add(5);               // returns false if already present
hs.contains(5);          // O(1)
hs.remove(5);            // returns true if it was present
hs.size();  hs.isEmpty();

Set<String> lhs = new LinkedHashSet<>();   // iterates in insertion order
// re-inserting an existing element does NOT change its position

// Set operations (these mutate the receiver, so copy first)
Set<Integer> union = new HashSet<>(a);   union.addAll(b);
Set<Integer> inter = new HashSet<>(a);   inter.retainAll(b);
Set<Integer> diff  = new HashSet<>(a);   diff.removeAll(b);

// Dedup a list while preserving order
List<Integer> unique = new ArrayList<>(new LinkedHashSet<>(list));
```

### `TreeSet`: sorted, Red-Black tree

```java
TreeSet<Integer> ts = new TreeSet<>();
TreeSet<Integer> desc = new TreeSet<>(Comparator.reverseOrder());
TreeSet<String> byLen = new TreeSet<>(Comparator.comparingInt(String::length)
                                                .thenComparing(Comparator.naturalOrder()));

ts.first();  ts.last();            // min / max
ts.floor(x);    // greatest element <= x  (or null)
ts.ceiling(x);  // smallest element >= x  (or null)
ts.lower(x);    // greatest element <  x
ts.higher(x);   // smallest element >  x
ts.pollFirst(); ts.pollLast();     // remove and return min / max
ts.headSet(x);  ts.tailSet(x);     // views: < x   and   >= x
ts.subSet(a, b);                   // [a, b)
ts.descendingSet();
```

- **Sorting rule:** elements must implement `Comparable`, or you pass a `Comparator` to the constructor, otherwise you get `ClassCastException`.
- **Trap:** `TreeSet`/`TreeMap` decide equality by `compare()`/`compareTo()` returning 0, **not** `equals()`. A comparator that returns 0 for two different objects silently drops one as a "duplicate".

### `Comparable` vs `Comparator`

```java
// Comparable: natural ordering, defined inside the class
class Student implements Comparable<Student> {
    int id; String name;
    public int compareTo(Student o) { return Integer.compare(this.id, o.id); }
}

// Comparator: external, flexible, can have many per type
Comparator<Student> byName = Comparator.comparing(s -> s.name);
Comparator<Student> multi  = Comparator.comparingInt((Student s) -> s.id)
                                       .thenComparing(s -> s.name)
                                       .reversed();
```

> **Never** write `(a, b) -> a - b` for general ints: subtraction can overflow. Use `Integer.compare(a, b)`.

---

## 4. Maps

| Implementation | Order | Time | Null key | Null values | Thread-safe? |
|---|---|---|---|---|---|
| `HashMap` | None | O(1) avg | One | Yes | No |
| `LinkedHashMap` | Insertion (or access) | O(1) avg | One | Yes | No |
| `TreeMap` | Sorted by key | O(log N) | **No** (natural ordering) | Yes | No |
| `ConcurrentHashMap` | None | O(1) avg | **No** | **No** | **Yes** |

### Essential `Map` API

```java
Map<String, Integer> m = new HashMap<>();
m.put("a", 1);                         // returns old value or null
m.get("a");                            // null if absent (careful unboxing to int -> NPE)
m.getOrDefault("z", 0);
m.containsKey("a");   m.containsValue(1);   // containsValue is O(N)
m.remove("a");
m.putIfAbsent("b", 2);

// Frequency counting (idiomatic)
m.merge("x", 1, Integer::sum);                       // m[x] += 1
m.put("y", m.getOrDefault("y", 0) + 1);              // older style

// Group-into-list pattern
Map<Integer, List<String>> groups = new HashMap<>();
groups.computeIfAbsent(len, k -> new ArrayList<>()).add(word);

// Iteration
for (Map.Entry<String, Integer> e : m.entrySet()) { e.getKey(); e.getValue(); }
for (String k : m.keySet()) {}
for (int v : m.values()) {}
m.forEach((k, v) -> System.out.println(k + "=" + v));
```

### `HashMap` internals

- **Structure:** array of buckets. Index is derived from `key.hashCode()`. Collisions are chained in a bucket.
- **Defaults:** initial capacity 16, load factor 0.75. When `size > capacity * 0.75`, it **resizes (doubles) and rehashes**, which costs O(N).
- **Treeification (Java 8+):** if one bucket's chain reaches 8 nodes **and** the table capacity is at least 64, the chain becomes a Red-Black tree, so the worst case drops from O(N) to O(log N). It converts back to a list when the bin shrinks to about 6. (These thresholds are implementation details of OpenJDK, so don't present them as part of the Java spec.)
- **`equals`/`hashCode` contract:** equal objects **must** have equal hash codes. Override both together, or `HashMap`/`HashSet` will misbehave.
- **Mutable keys:** changing a key's fields after insertion can make the entry unfindable. Prefer immutable keys (`String`, `Integer`, records).
- **Pre-size to avoid rehashing:** `new HashMap<>(expectedSize * 4 / 3 + 1)`.

### `LinkedHashMap`

```java
Map<String, Integer> ordered = new LinkedHashMap<>();   // insertion order

// LRU cache in about 6 lines (access-order mode)
class LRU<K, V> extends LinkedHashMap<K, V> {
    private final int cap;
    LRU(int cap) { super(16, 0.75f, true); this.cap = cap; }   // true = access order
    @Override protected boolean removeEldestEntry(Map.Entry<K, V> e) { return size() > cap; }
}
```

### `TreeMap`: sorted keys, Red-Black tree

```java
TreeMap<Integer, String> tm = new TreeMap<>();
tm.firstKey();  tm.lastKey();
tm.firstEntry(); tm.lastEntry();
tm.floorKey(x); tm.ceilingKey(x);      // <= x  and  >= x
tm.lowerKey(x); tm.higherKey(x);       // <  x  and  >  x
tm.pollFirstEntry(); tm.pollLastEntry();
tm.headMap(x); tm.tailMap(x); tm.subMap(a, b);   // views
tm.descendingMap();
```

### `ConcurrentHashMap` (brief)

- Thread-safe without locking the whole map. In Java 8+ it uses CAS plus fine-grained locking on individual bins, instead of the old segment-based design.
- **No `null` keys or values** (a `null` return from `get` must mean "absent").
- Use its **atomic** methods for check-then-act logic: `putIfAbsent`, `compute`, `merge`, `computeIfAbsent`. A manual `if (!m.containsKey(k)) m.put(k, v)` is still a race.
- Iterators are weakly consistent (they never throw `ConcurrentModificationException`). `size()` is only an estimate under concurrent updates.
- Prefer it over `Hashtable` or `Collections.synchronizedMap(...)`.

---

## 5. Queues, Deques & Priority Queues

### `Queue` method pairs

| Operation | Throws exception | Returns special value |
|---|---|---|
| Insert | `add(e)` | `offer(e)` |
| Remove head | `remove()` | `poll()` (null if empty) |
| Examine head | `element()` | `peek()` (null if empty) |

```java
Queue<Integer> q = new ArrayDeque<>();    // or new LinkedList<>()
q.offer(1);  q.offer(2);
q.peek();    // 1
q.poll();    // 1
q.isEmpty();
```

### `ArrayDeque`: use it for both stack and queue

```java
Deque<Integer> dq = new ArrayDeque<>();

// As a STACK (LIFO), operating at the head
dq.push(1);  dq.push(2);
dq.peek();   // 2
dq.pop();    // 2

// As a QUEUE (FIFO)
dq.offerLast(1);  dq.pollFirst();        // same as offer / poll

// Both ends
dq.offerFirst(x); dq.offerLast(x);
dq.pollFirst();   dq.pollLast();
dq.peekFirst();   dq.peekLast();
```

| Why `ArrayDeque` | Detail |
|---|---|
| Beats legacy `Stack` | `Stack` extends `Vector`, so every call is synchronized (needless overhead) and it exposes index methods that break stack semantics. The Java docs themselves recommend `Deque`. |
| Beats `LinkedList` | Resizable circular array: less memory, better cache behavior, no node allocation. |
| Limitations | **No `null` elements**, not thread-safe, no index access. |

> Mixing `push`/`pop` (head) with `offer`/`poll` (head vs tail) is confusing. `push` adds to the **front** and `offer` adds to the **end**. Stick to one style per structure.

### `PriorityQueue`: binary heap

```java
PriorityQueue<Integer> minH = new PriorityQueue<>();                          // min-heap (default)
PriorityQueue<Integer> maxH = new PriorityQueue<>(Collections.reverseOrder()); // max-heap
PriorityQueue<int[]>   byCol = new PriorityQueue<>((a, b) -> Integer.compare(a[1], b[1]));
PriorityQueue<int[]>   init  = new PriorityQueue<>(11, Comparator.comparingInt(a -> a[0]));

pq.offer(x);     // O(log N)
pq.peek();       // O(1)
pq.poll();       // O(log N), null if empty
pq.size();
new PriorityQueue<>(collection);   // heapify: O(N)
```

| Property | Detail |
|---|---|
| Order | Only the head is guaranteed to be min (or max). **Iteration and `toString()` are NOT sorted.** |
| `remove(Object)` / `contains` | O(N) |
| Nulls | Not allowed |
| Ties | Not stable (equal-priority order is unspecified) |
| Thread-safe version | `PriorityBlockingQueue` |

**Top-K pattern:** to find the K largest, keep a **min-heap of size K**. Push each element, and if `size > K`, `poll()`. This takes O(N log K).

---

## 6. Utility Classes: `Collections` & `Arrays`

### `Collections`

```java
List<Integer> list = new ArrayList<>(List.of(3, 1, 2));

Collections.sort(list);                          // ascending, stable (merge-sort variant)
Collections.sort(list, Collections.reverseOrder());
list.sort(Comparator.naturalOrder());            // same thing, more modern style

Collections.reverse(list);
Collections.shuffle(list);
Collections.swap(list, 0, 1);
Collections.max(list);   Collections.min(list);
Collections.frequency(list, 3);                  // O(N) count of equal elements
Collections.nCopies(5, 0);                       // immutable list of 5 zeros
Collections.emptyList();

// Binary search: list MUST be sorted with the same ordering
int idx = Collections.binarySearch(list, key);
// idx >= 0 -> found at that index
// idx <  0 -> not found; insertion point = -(idx) - 1
// with duplicates, WHICH matching index is returned is unspecified
```

### Unmodifiable collections

```java
List<Integer> view = Collections.unmodifiableList(list);   // read-only VIEW of list
list.add(4);                                               // view sees this change!
view.add(5);                                               // throws UnsupportedOperationException

List<Integer> imm  = List.of(1, 2, 3);                     // Java 9+: truly immutable, no nulls
Set<Integer>  imms = Set.of(1, 2);                         // Java 9+
Map<String,Integer> imm2 = Map.of("a", 1, "b", 2);         // Java 9+
List<Integer> copy = List.copyOf(list);                    // Java 10+: immutable snapshot
```

| Method | Behavior |
|---|---|
| `Collections.unmodifiableList(l)` | **View**: changes to the original show through |
| `List.of(...)` / `List.copyOf(l)` | **Immutable**: no nulls; `of` throws on duplicate set elements/map keys |
| `Arrays.asList(arr)` | **Fixed-size**: can `set`, cannot `add`/`remove`; writes go through to the array |
| `Collections.synchronizedList(l)` | Wraps with synchronized methods |

### `Arrays`

```java
int[] arr = {5, 2, 9};
Arrays.sort(arr);                          // primitives: dual-pivot quicksort
Arrays.sort(arr, 0, 2);                    // sort range [0, 2)
Integer[] boxed = {5, 2, 9};
Arrays.sort(boxed, Collections.reverseOrder());   // comparators need OBJECT arrays, not int[]

Arrays.toString(arr);                      // "[2, 5, 9]"
Arrays.deepToString(grid);                 // for 2D arrays
Arrays.fill(arr, -1);
Arrays.equals(a, b);   Arrays.deepEquals(g1, g2);
Arrays.copyOf(arr, newLen);                // pads with 0 / truncates
Arrays.copyOfRange(arr, from, to);         // [from, to)
Arrays.binarySearch(arr, key);             // same return convention as Collections
Arrays.asList(1, 2, 3);                    // fixed-size list view
Arrays.stream(arr).sum();                  // IntStream
```

**Common conversions**
```java
List<Integer> l = Arrays.stream(arr).boxed().collect(Collectors.toList());   // int[] -> List
int[] back = list.stream().mapToInt(Integer::intValue).toArray();            // List -> int[]
Integer[] objs = list.toArray(new Integer[0]);                               // List -> Integer[]
```

> **Trap:** `Arrays.asList(intArray)` gives a `List<int[]>` with ONE element (the whole array), not a list of ints. It only expands for object arrays such as `Integer[]`.
> **Trap:** `arr.length` (field) vs `list.size()` vs `str.length()` (methods).

---

## Quick Chooser

| Need | Pick |
|---|---|
| General dynamic array | `ArrayList` |
| Stack | `ArrayDeque` (`push`/`pop`/`peek`) |
| Queue / BFS | `ArrayDeque` (`offer`/`poll`) |
| Membership, dedup, no order | `HashSet` |
| Dedup but keep insertion order | `LinkedHashSet` |
| Sorted set, floor/ceiling queries | `TreeSet` |
| Fast key-value lookup | `HashMap` |
| Key-value with predictable iteration / LRU | `LinkedHashMap` |
| Sorted keys, range queries | `TreeMap` |
| Shared map across threads | `ConcurrentHashMap` |
| Repeated min/max extraction, top-K, Dijkstra | `PriorityQueue` |

## Interview Pitfalls Checklist

- [ ] `list.remove(int)` vs `list.remove(Object)` on `List<Integer>`
- [ ] Modifying a collection while iterating it (`ConcurrentModificationException`)
- [ ] Overriding `equals` without `hashCode` (or the reverse) on hash keys
- [ ] Mutable objects used as `HashMap` keys
- [ ] `map.get(k)` returning `null` and then auto-unboxing into an `int` (NPE)
- [ ] Assuming `HashMap`/`HashSet` iteration order is stable or sorted
- [ ] Printing a `PriorityQueue` and expecting sorted output
- [ ] `a - b` in a comparator (integer overflow): use `Integer.compare`
- [ ] Using legacy `Stack`, `Vector`, `Hashtable` instead of modern equivalents
- [ ] `Arrays.asList(int[])` producing a one-element list
- [ ] `unmodifiableList` is a view, not a copy
- [ ] `TreeSet` treating "comparator returns 0" as "duplicate"
- [ ] `null` in `ArrayDeque`, `TreeMap` keys, or `ConcurrentHashMap`

---

*Accuracy notes: the growth factor of `ArrayList`, the `HashMap` treeify thresholds (8 / 6 / 64), and the "usually faster than `LinkedList`" guidance describe typical OpenJDK behavior, not guarantees of the Java spec. `List.of`/`Set.of`/`Map.of` need Java 9+, and `List.copyOf` needs Java 10+. Check against the Javadoc for your JDK version if an interviewer or judge uses an older release.*
