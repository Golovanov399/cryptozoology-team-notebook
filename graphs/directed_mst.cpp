vector<int> min_edges;

// RETURNS: value of directed MST with root in root
// ids of min egdes are pushed into min_edges
// WARNING: DO NOT FORGET TO FILL edge.id !!! (algorithm reports these values)
li findMst(vector<edge>& edges, int n, int root) {
    li res = 0;

    const li INF = 1e18;
    vector<li> minCost(n, INF);
    vector<int> id_edge(n, -1);
    
    for (int i = 0; i < edges.size(); i++)
        edges[i].local_id = i;
    
    for (edge& e: edges) {
        if (e.from == e.to || e.to == root) continue;
        
        if (minCost[e.to] > e.cost) {
            minCost[e.to] = e.cost;
            id_edge[e.to] = e.id;
        }
    }

    for (int v = 0; v < n; v++)
        if (v != root) {
            res += minCost[v];
        }

    vector<edge> zero;
    for (edge& e: edges) {
        if (e.from == e.to || e.to == root) continue;
        
        e.cost -= minCost[e.to];
        if (e.cost == 0)
            zero.push_back(e);
    }

    vector<vector<tuple<int, int, int>>> zero_to(n), zero_to_rev(n);
    for (edge& e: zero) {
        zero_to[e.from].emplace_back(e.to, e.id, e.local_id);
        zero_to_rev[e.to].emplace_back(e.from, e.id, e.local_id);
    }

    vector<char> used(n, false);
    vector<int> out_order;

    vector<int> can_min;
    function<void(int)> dfs = [&](int v) {
        used[v] = true;
        for (auto ed: zero_to[v]) {
            int u = get<0>(ed);

            if (!used[u]) {
                dfs(u);
                can_min.push_back(get<1>(ed));
            }
        }
        out_order.push_back(v);
    };

    dfs(root);

    bool fail = false;
    for (int v = 0; v < n; v++)
        if (!used[v]) {
            fail = true;
            dfs(v);
        }

    if (!fail) {
        min_edges = can_min;
        answer += res;
        return res;
    }

    reverse(all(out_order));

    vector<int> color(n, -1);

    int curColor = 0;

    function<void(int)> colorDfs = [&](int v) {
        color[v] = curColor;

        for (auto ed: zero_to_rev[v]) {
            int u = get<0>(ed);
            if (color[u] == -1) {
                colorDfs(u);
                min_edges.push_back(get<2>(ed));
            }
        }
    };

    for (int v: out_order) {
        if (color[v] == -1) {
            colorDfs(v);
            curColor++;
        }
    }

    vector<edge> new_edges;
    for (int i = 0; i < edges.size(); i++) {
        edge& e = edges[i];
        if (e.from == e.to || e.to == root) continue;
        
        if (color[e.to] != color[e.from]) {
            edge new_e = edge { color[e.from], color[e.to], e.cost };
            new_e.id = i;
            new_edges.push_back(new_e);
        }
    }

    answer += res;
    li mst_res = findMst(new_edges, curColor, color[root]);
    res += mst_res;
    
    can_min.clear();
    used.assign(n, false);

    function<void(int)> sc_dfs = [&](int v) {
        used[v] = true;
        for (auto ed: zero_to[v]) {
            int u = get<0>(ed);
            if (color[u] == color[v] && !used[u]) {
                assert(get<1>(ed) >= 0);
                min_edges.push_back(get<2>(ed));
                sc_dfs(u);
            }
        }
    };

    for (int i = 0; i < min_edges.size(); i++) {
        int id = min_edges[i];
        edge& e = edges[id];
        can_min.push_back(e.id);

        sc_dfs(e.to);
    }

    sc_dfs(root);

    min_edges = can_min;
    return res;
}
