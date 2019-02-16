struct SuffixTree {
  vector<vector<pair<int, int>>> g;
  vector<int> is_leaf, max_len;
  vector<int> leaves_before;
  vector<int> cnt_leaves;
  int n;
  SuffixTree(vector<int> s) {
    s.push_back(-1);
    reverse(all(s));
    n = s.size();
    auto automata = SuffixAutomaton(s);
    g.resize(automata.states.size());
    is_leaf.resize(automata.states.size(), 0);
    max_len.assign(g.size(), 0);
    cnt_leaves.assign(g.size(), 0);
    leaves_before.assign(g.size(), 0);
    for (int v = 1; v < automata.states.size(); ++v) {
      int p = automata.states[v].link;
      max_len[v] = automata.states[v].maxLen;
      is_leaf[v] = automata.states[v].firstPos + 1 == automata.states[v].maxLen;
      int transition_pos = automata.states[v].lastPos - automata.states[p].maxLen;
      g[p].push_back({s[transition_pos], v});
    }
    for (auto& vec : g) {
      sort(all(vec));
    }
    vector<int> new_leaves;
    for (int i = 0; i < g.size(); ++i) {
      vector<int> to_erase;
      for (int j = 0; j < g[i].size(); ++j) {
        int to = g[i][j].second;
        if (is_leaf[to]) {
          --max_len[to];
          if (max_len[to] == max_len[i]) {
            to_erase.push_back(j);
            is_leaf[to] = false;
            if (i > 0) {
              new_leaves.push_back(i);
            }
          }
        }
      }
      vector<pair<int, int>> copy_g;
      int uk = 0;
      for (int j = 0; j < g[i].size(); ++j) {
        if (uk < to_erase.size() && j == to_erase[uk]) {
          ++uk;
          continue;
        }
        copy_g.push_back(g[i][j]);
      }
      copy_g.swap(g[i]);
    }
    for (int v : new_leaves) {
      is_leaf[v] = 1;
    }
  }
};
