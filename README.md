# Список проебонов на контесте

## Всегда выводим ответ на запрос!

Неправильно:

```c++
while (q--) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    if (!dsu.merge(u, v)) {
        // ниче ж не поменялось)))))) можно сделать continue))))))
        continue;
    }
    make_some_logic(u, v);
    cout << get_cur_ans() << "\n";
}
```

Правильно:

```c++
while (q--) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    if (dsu.merge(u, v)) {
        make_some_logic(u, v);
    }
    cout << get_cur_ans() << "\n";
}
```

## `m` рёбер, а не `n`.

Неправильно:

```c++
int n, m;
cin >> n >> m;
vector<vector<int>> a(n);
for (int i = 0; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    a[u].push_back(v);
    a[v].push_back(u);
}
```

Правильно:

```c++
int n, m;
cin >> n >> m;
vector<vector<int>> a(n);
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    a[u].push_back(v);
    a[v].push_back(u);
}
```

## Не забываем построить дерево отрезков после инициализации листьев.

Неправильно:
```c++
for (int i = 0; i < n; ++i) {
    tree.set(i, a[i]);
}
for (int i = 0; i < Q; ++i) {
    int pos, val;
    cin >> pos >> val;
    tree.update(pos, val);
}
```
Правильно:
```c++
for (int i = 0; i < n; ++i) {
    tree.set(i, a[i]);
}
tree.build();
for (int i = 0; i < Q; ++i) {
    int pos, val;
    cin >> pos >> val;
    tree.update(pos, val);
}
```

## Лучше `struct` с понятными названиями полей, а не `std::pair`.

Неправильно:

```c++
set<pair<int, int>> a;
for (int i = 0; i < n; ++i) {
    int pos, val;
    cin >> pos >> val;
    a.insert({pos, val});
}
sort(all(a));

int q;
cin >> q;
while (q--) {
    int pos, val;
    cin >> pos >> val;
    auto it = a.lower_bound({pos, 0});
    if (it != a.end() && it->first > val) { // эээ ну в сете же по first сортим в 1ю очередь
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}
```

Правильно:

```c++
struct Shit {
    int pos;
    int val;
    
    bool operator <(const Shit& ot) const {
        return make_pair(pos, val) < make_pair(ot.pos, ot.val);
    }
}

set<Shit> a;
for (int i = 0; i < n; ++i) {
    int pos, val;
    cin >> pos >> val;
    a.insert({pos, val});
}
sort(all(a));

int q;
cin >> q;
while (q--) {
    int pos, val;
    cin >> pos >> val;
    auto it = a.lower_bound({pos, 0});
    if (it != a.end() && it->val > val) {   // хуй проебёшься
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}
```

## Перенумерация в эйлеровом обходе.
Неправильно:

```c++
for (int i = 0; i < n; ++i) {
    tree.update(i, 1);
}
for (int i = 0; i < n; ++i) {
    cout << tree.get_val(i) << endl;
}
```

Правильно:

```c++
for (int i = 0; i < n; ++i) {
    tree.update(tin[i], 1);
}
for (int i = 0; i < n; ++i) {
    cout << tree.get_val(tin[i]) << endl;
}
```

## `vector<char>` хранит числа до 255.

Неправильно:

```c++
vector<char> used(n), num_comp(n);
int cur = 0;
for (int i = 0; i < n; ++i) {
    if (!used[i]) {
        dfs(i, cur++);
    }
}
```

Правильно:

```c++
vector<char> used(n);
vector<int> num_comp(n);
int cur = 0;
for (int i = 0; i < n; ++i) {
    if (!used[i]) {
        dfs(i, cur++);
    }
}
```

## `bool f()` возвращает `bool`.

Неправильно:

```c++
bool occurs(const string& s, const string& t) {
    for (int i = 0; i + (int)s.length() <= (int)t.length(); ++i) {
        // падажжи ебана
        // если содержится, то нужен индекс
        if (t.substr(i, s.length()) == s) {
            return i;
        }
    }
    // иначе пускай будет -1
    return -1;
}
```

Правильно:

```c++
int occurs(const string& s, const string& t) {
...
```

## Индексы в `dsu` до `n`, а не до `num_comps`.

## В `merge` для вершин дерева отрезков `push_val = UNDEFINED`.
Неправильно:

```c++
Node merge(const Node& q, const Node& w) {
    Node res; // или res = q
    res.min = min(q.min, w.min); // или if (w.min < res.min) res = w
    return res;
}
```
Правильно:
```c++
Node merge(const Node& q, const Node& w) {
    Node res;
    res.push_add = 0; // или в объявлении res = {}, если в конструкторе по умолчанию прописано заполнение
    res.min = min(q.min, w.min);
    return res;
}
```
