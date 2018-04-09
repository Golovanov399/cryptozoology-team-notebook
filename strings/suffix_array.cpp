void Build(const string& init, vector<int>& suffArray, vector<int>& lcp) {
    string s = init;
    s.push_back(char(0));
    int n = s.size();
    vector<int> head(max(n, 256));
    vector<int> color(n);
    vector<int> colorSub(n);
    vector<int> suffArraySub(n);
    lcp.resize(n);
    suffArray.resize(n);

    for (int i = 0; i < s.size(); ++i) {
        ++head[s[i]];
    }
    for (int i = 1; i < 256; ++i) {
        head[i] += head[i - 1];
    }
    for (int i = 255; i > 0; --i) {
        head[i] = head[i - 1];
    }
    head[0] = 0;
    for (int i = 0; i < s.size(); ++i) {
        suffArray[head[s[i]]] = i;
        ++head[s[i]];
    }
    int numClasses = 1;
    head[0] = 0;
    for (int i = 1; i < s.size(); ++i) {
        if (s[suffArray[i - 1]] != s[suffArray[i]]) {
            ++numClasses;
            head[numClasses - 1] = i;
        }
        color[suffArray[i]] = numClasses - 1;
    }
    for (int k = 1; k < s.size(); k *= 2) {
        for (int i = 0; i < s.size(); ++i) {
            int first = suffArray[i] - k;
            if (first < 0) {
                first += s.size();
            }
            suffArraySub[head[color[first]]] = first;
            ++head[color[first]];
        }
        suffArray = suffArraySub;

        int second;
        pair<int, int> prevClasses, curClasses;
        curClasses = { -1, 0 };
        numClasses = 0;

        for (int i = 0; i < s.size(); ++i) {
            prevClasses = curClasses;

            second = suffArray[i] + k;
            if (second >= s.size()) {
                second -= s.size();
            }
            curClasses = { color[suffArray[i]], color[second] };

            if (curClasses != prevClasses) {
                ++numClasses;
                head[numClasses - 1] = i;
            }
            colorSub[suffArray[i]] = numClasses - 1;
        }

        color = colorSub;

        if (numClasses == s.size())
            break;
    }
    vector <int> pos;
    int curLcp = 0;
    pos.resize(s.size());
    for (int i = 0; i < s.size(); ++i) {
        pos[suffArray[i]] = i;
    }
    lcp.resize(s.size());
    for (int i = 0; i < s.size(); ++i) {
        if (pos[i] == s.size() - 1) {
            lcp[pos[i]] = 0;
            curLcp = 0;
            continue;
        }

        while (s[(i + curLcp) % s.size()] == s[(suffArray[pos[i] + 1] + curLcp) % s.size()]) {
            ++curLcp;
        }
        lcp[pos[i]] = curLcp;

        --curLcp;
        if (curLcp < 0)
            curLcp = 0;
    }
}

void BuildSparseTable(const vector <int>& a, vector < vector <int> >& sparseTable) {
    int logSize = 0;
    while ((1 << logSize) < a.size()) {
        ++logSize;
    }
    logSize = 19; // <-- THINK HERE!
    sparseTable.assign(a.size(), vector <int> (logSize + 1));

    for (int i = 0; i < a.size(); ++i) {
        sparseTable[i][0] = a[i];
    }

    for (int k = 1; k <= logSize; ++k) {
        for (int i = 0; i + (1 << k) <= a.size(); ++i) {
            sparseTable[i][k] = min(sparseTable[i][k - 1], sparseTable[i + (1 << (k - 1))][k - 1]);
        }
    }
}

int GetMin(int l, int r, const vector < vector <int> >& sparseTable) {
    assert(l < r);
    int sz = 31 - __builtin_clz(r - l);
    return min(sparseTable[l][sz], sparseTable[r - (1 << sz)][sz]);
}

void solve(__attribute__((unused)) bool read) {
    string s;
    cin >> s;
    int n = s.length();
    vector<int> suffArray, lcp;
    Build(s, suffArray, lcp);
    suffArray.erase(suffArray.begin());
    lcp.erase(lcp.begin());
    vector<int> pos_in_array(n);
    for (int i = 0; i < suffArray.size(); ++i) {
        pos_in_array[suffArray[i]] = i;
    }
    vector<vector<int>> sparse;
    BuildSparseTable(lcp, sparse);

}
