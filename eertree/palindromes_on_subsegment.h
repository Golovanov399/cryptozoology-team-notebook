struct Node {
    int len;
    int link;
    vector<int> trans;
    bool all_equal;
    Node() {
        len = 0;
        link = 0;
        trans.assign(26, -1);
        all_equal = true;
    }
};

struct Eertree {
    vector<Node> nodes;
    vector<int> one_len;
    Eertree() {
        nodes.push_back(Node());
        one_len.assign(26, -1);
    }
    vector<int> feed_string(const string& s) {
        int v = 0;
        int n = s.length();
        vector<int> state(n);
        for (int i = 0; i < s.length(); ++i) {
            int c = s[i] - 'a';
            bool flag = false;
            while (v) {
                if (nodes[v].all_equal && s[i] == s[i - 1]) {
                    if (nodes[v].trans[c] == -1) {
                        nodes[v].trans[c] = nodes.size();
                        nodes.push_back(Node());
                        nodes.back().len = nodes[v].len + 1;
                        nodes.back().all_equal = true;
                        nodes.back().link = v;
                    }
                    v = nodes[v].trans[c];
                    flag = true;
                    break;
                }
                if (i > nodes[v].len && s[i] == s[i - nodes[v].len - 1]) {
                    if (nodes[v].trans[c] == -1) {
                        nodes[v].trans[c] = nodes.size();
                        nodes.push_back(Node());
                        nodes.back().len = nodes[v].len + 2;
                        nodes.back().link = -1;
                        nodes.back().all_equal = false;
                        int cur_v = nodes[v].link;
                        while (cur_v) {
                            if (nodes[cur_v].trans[c] != -1) {
                                int cand = nodes[cur_v].trans[c];
                                if (s[i] == s[i - nodes[cand].len + 1]) {
                                    nodes.back().link = nodes[cur_v].trans[c];
                                    break;
                                }
                            }
                            cur_v = nodes[cur_v].link;
                        }
                        if (nodes.back().link == -1) {
                            if (nodes[cur_v].trans[c] != -1) {
                                nodes.back().link = nodes[cur_v].trans[c];
                            } else {
                                nodes[cur_v].link = 0;
                            }
                        }
                    }
                    v = nodes[v].trans[c];
                    flag = true;
                    break;
                }
                v = nodes[v].link;
            }
            if (!flag) {
                if (one_len[c] == -1) {
                    nodes[v].trans[c] = nodes.size();
                    nodes.push_back(Node());
                    nodes.back().len = 1;
                    one_len[c] = nodes[v].trans[c];
                    nodes.back().all_equal = true;
                    nodes.back().link = 0;
                } else {
                    nodes[v].trans[c] = one_len[c];
                }
                v = nodes[v].trans[c];
            }
            state[i] = v;
        }
        return state;
    }

    void enclose() {
        for (int v = 0; v < nodes.size(); ++v) {
            for (int c = 0; c < 26; ++c) {
                if (nodes[v].trans[c] == -1) {
                    int cur_v = nodes[v].link;
                    while (true) {
                        if (nodes[cur_v].trans[c] != -1) {
                            nodes[v].trans[c] = nodes[cur_v].trans[c];
                            break;
                        }
                        if (cur_v == 0) {
                            nodes[v].trans[c] = 0;
                            break;
                        }
                        cur_v = nodes[cur_v].link;
                    }
                }
            }
        }
    }

};

struct Query {
    int l, r;
    int id;
    bool operator < (const Query& ot) const {
        if (r != ot.r) {
            return r < ot.r;
        }
        return l < ot.l;
    }
};

void solve(bool read) {
    string s;
    cin >> s;
    Eertree tree;
    tree.feed_string(s);
    tree.enclose();
    int Q;
    cin >> Q;
    int n = s.length();
    int block_size = max((int)(sqrt(n) * 1.5), 1);
    int blocks = (n - 1) / block_size + 1;
    for (int i = 0; i < Q; ++i) {
        Query cur;
        cin >> cur.l >> cur.r;
        --cur.l;
        cur.id = i;
        q[cur.l / block_size].push_back(cur);
    }
    vector<int> ans(Q);
    vector<int> used(tree.nodes.size(), 0);
    vector<int> left_used(tree.nodes.size(), 0);
    int TIMER = 0;
    int LEFT_TIMER = 0;
    for (int block = 0; block < blocks; ++block) {
        sort(all(q[block]));
        int right_border = min((block + 1) * block_size, n);
        int uk = 0;
        while (uk < q[block].size() && q[block][uk].r < right_border) {
            ++TIMER;
            int res = 0;
            int v = 0;
            for (int pos = q[block][uk].l; pos < q[block][uk].r; ++pos) {
                v = tree.nodes[v].trans[s[pos] - 'a'];
                if (s[pos] != s[pos - tree.nodes[v].len + 1]) {
                    v = tree.nodes[v].link;
                }
                if (tree.nodes[v].len > pos + 1 - q[block][uk].l) {
                    v = tree.nodes[v].link;
                }
                if (used[v] != TIMER) {
                    ++res;
                    used[v] = TIMER;
                }
            }
            ans[q[block][uk].id] = res;
            ++uk;
        }

        int cur_r = right_border;
        int overall_pals = 0;
        int right_state = 0;
        int left_state = 0;
        ++TIMER;
        while (uk < q[block].size()) {
            while (cur_r < q[block][uk].r) {
                right_state = tree.nodes[right_state].trans[s[cur_r] - 'a'];
                if (s[cur_r] != s[cur_r - tree.nodes[right_state].len + 1]) {
                    right_state = tree.nodes[right_state].link;
                }
                if (tree.nodes[right_state].len > cur_r + 1 - right_border) {
                    right_state = tree.nodes[right_state].link;
                }
                if (used[right_state] != TIMER) {
                    ++overall_pals;
                    used[right_state] = TIMER;
                }
                if (tree.nodes[right_state].len == cur_r + 1 - right_border) {
                    left_state = right_state;
                }
                ++cur_r;
            }
            ++LEFT_TIMER;
            int cur_l = right_border;
            int cur_left_state = left_state;
            int cur_res = overall_pals;
            while (cur_l > q[block][uk].l) {
                --cur_l;
                cur_left_state = tree.nodes[cur_left_state].trans[s[cur_l] - 'a'];
                if (s[cur_l] != s[cur_l + tree.nodes[cur_left_state].len - 1]) {
                    cur_left_state = tree.nodes[cur_left_state].link;
                }
                if (tree.nodes[cur_left_state].len > cur_r - cur_l) {
                    cur_left_state = tree.nodes[cur_left_state].link;
                }
                if (used[cur_left_state] != TIMER && left_used[cur_left_state] != LEFT_TIMER) {
                    ++cur_res;
                    left_used[cur_left_state] = LEFT_TIMER;
                }
            }
            ans[q[block][uk].id] = cur_res;
            ++uk;
        }
    }
    for (int i = 0; i < Q; ++i) {
            cout << ans[i] << "\n";
    }
}
