using cost_type = int;
const cost_type COST_INF = (cost_type)1e9;
const int FLOW_INF = (int)1e9;

struct MinCost {
    explicit MinCost(int n) {
        g.resize(n);
    }

    struct edge {
        int from, to;
        int cap;
        cost_type cost;
        int flow;
    };

    vector<edge> edges;
    vector<vector<int>> g;

    void add_edge(int from, int to, cost_type cur_cost, int cap) {
        edge e = {from, to, cap, cur_cost, 0};
        g[from].push_back(edges.size());
        edges.push_back(e);
        edge e2 = {to, from, 0, -cur_cost, 0};
        g[to].push_back(edges.size());
        edges.push_back(e2);
    }

    pair<int, cost_type> min_cost(int n, int s, int t, int max_flow_value = FLOW_INF) {
        cost_type cost = 0;
        int flow = 0;

        vector<int> p(n);
        vector<cost_type> d(n, 0);
        vector<int> to_add;
        while (flow < max_flow_value) {
            p.assign(n, -1);
            d.assign(n, COST_INF);
            d[s] = 0;
            set<pair<cost_type, int>> q;
            q.insert({0, s});
            vector<char> used(n, false);
            while (!q.empty()) {
                int v = q.begin()->second;
                q.erase(q.begin());
                used[v] = true;
                for (int i : g[v]) {
                    auto& e = edges[i];
                    if (e.cap == e.flow || used[e.to]) {
                        continue;
                    }
                    cost_type new_d = d[v] + e.cost;
                    if (d[e.to] > new_d) {
                        q.erase({d[e.to], e.to});
                        d[e.to] = new_d;
                        q.insert({d[e.to], e.to});
                        p[e.to] = i;
                    }
                }
            }
            if (p[t] == -1) {
                return {-1, 0};
            }
            int add_flow = max_flow_value - flow;
            int cur = t;
            to_add.clear();
            int add_cost = 0;
            while (cur != s) {
                auto& e = edges[p[cur]];
                add_flow = min(add_flow, e.cap - e.flow);
                to_add.push_back(p[cur]);
                cur = e.from;
                add_cost += e.cost;
            }
            assert(add_flow > 0);
            flow += add_flow;
            cost += add_flow * add_cost;
            for (int x : to_add) {
                edges[x].flow += add_flow;
                edges[x ^ 1].flow -= add_flow;
            }
        }

        int TIMER = 0;
        vector<int> used_timer(n, 0);
        vector<char> used(n, false);
        vector<int> cur_edges;
        vector<int> edges_to_add;
        while (true) {
            p.assign(n, -1);
            d.assign(n, COST_INF);
            bool found = false;
            int iter = 0;
            for (int st = 0; st < s; ++st) {
                if (d[st] != COST_INF) {
                    continue;
                }
                ++iter;
                d[st] = 0;
                vector<int> q, new_q;
                q.push_back(st);
                for (int it = 0; it < n; ++it) {
                    ++TIMER;
                    int changed = -1;
                    for (int v : q) {
                        for (int i : g[v]) {
                            edge &e = edges[i];
                            if (e.cap == e.flow)
                                continue;
                            cost_type new_d = d[v] + e.cost;
                            if (d[e.to] > new_d) {
                                d[e.to] = new_d;
                                p[e.to] = i;
                                changed = e.to;
                                if (used_timer[e.to] != TIMER) {
                                    used_timer[e.to] = TIMER;
                                    new_q.push_back(e.to);
                                }
                            }
                        }
                    }
                    if (changed == -1) {
                        break;
                    }
                    sort(all(new_q));
                    q.swap(new_q);
                    new_q.clear();
                    if (d[st] < 0) {
                        changed = st;
                        it = n - 1;
                    }
                    if (it == n - 1) {
                        found = true;
                        int bad_end = changed;
                        used.assign(n, false);
                        int cur = bad_end;
                        cur_edges.clear();
                        while (!used[cur]) {
                            used[cur] = true;
                            cur_edges.push_back(p[cur]);
                            cur = edges[p[cur]].from;
                        }
                        edges_to_add.clear();
                        while (edges[cur_edges.back()].to != cur) {
                            edges_to_add.push_back(cur_edges.back());
                            cur_edges.pop_back();
                        }
                        edges_to_add.push_back(cur_edges.back());
                        int add_cost = 0, add_flow = FLOW_INF;
                        for (auto e_id : edges_to_add) {
                            add_flow = min(add_flow, edges[e_id].cap - edges[e_id].flow);
                            add_cost += edges[e_id].cost;
                        }
                        cost += add_cost * add_flow;
                        assert(add_flow > 0);
                        assert(add_cost < 0);
                        for (auto e_id : edges_to_add) {
                            edges[e_id].flow += add_flow;
                            edges[e_id ^ 1].flow -= add_flow;
                        }
                    }
                }
            }
            if (!found) {
                break;
            }
        }
        return make_pair(flow, cost);
    }
};
