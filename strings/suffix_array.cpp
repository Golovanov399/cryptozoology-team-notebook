
void Build(const string& init, vector <int>& suffArray, vector <int>& lcp) {
    string s = init;
    s.push_back(char(0));
    vector <int> head;
    vector <int> color;
    vector <int> colorSub;
    vector <int> suffArraySub;
    head.assign(max((int)s.size(), 256), 0);
    suffArray.resize(s.size());
    color.resize(s.size());
    colorSub.resize(s.size());
    suffArraySub.resize(s.size());
    lcp.resize(s.size());

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
    int numberOfClasses = 1;
    head[0] = 0;
    for (int i = 1; i < s.size(); ++i) {
        if (s[suffArray[i - 1]] != s[suffArray[i]]) {
            ++numberOfClasses;
            head[numberOfClasses - 1] = i;
        }
        color[suffArray[i]] = numberOfClasses - 1;
    }
    for (int k = 1; k < s.size(); k *= 2) {
        for (int i = 0; i < s.size(); ++i) {
            int firstPartBeginning = suffArray[i] - k;
            if (firstPartBeginning < 0) {
                firstPartBeginning += s.size();
            }
            suffArraySub[head[color[firstPartBeginning]]] = firstPartBeginning;
            ++head[color[firstPartBeginning]];
        }
        suffArray = suffArraySub;

        int secondPartBeginning;
        pair <int, int> prevSuffClasses, curSuffClasses;
        curSuffClasses = make_pair(-1, 0);
        numberOfClasses = 0;

        for (int i = 0; i < s.size(); ++i) {
            prevSuffClasses = curSuffClasses;

            secondPartBeginning = suffArray[i] + k;
            if (secondPartBeginning >= s.size()) {
                secondPartBeginning -= s.size();
            }
            curSuffClasses = make_pair(color[suffArray[i]], color[secondPartBeginning]);

            if (curSuffClasses != prevSuffClasses) {
                ++numberOfClasses;
                head[numberOfClasses - 1] = i;
            }
            colorSub[suffArray[i]] = numberOfClasses - 1;
        }

        color = colorSub;

        if (numberOfClasses == s.size())
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
    logSize = 19;
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
    vector<int> suff_array, lcp;
    Build(s, suff_array, lcp);
    suff_array.erase(suff_array.begin());
    lcp.erase(lcp.begin());
    vector<int> pos_in_array(n);
    for (int i = 0; i < suff_array.size(); ++i) {
        pos_in_array[suff_array[i]] = i;
    }
    vector<vector<int>> sparse;
    BuildSparseTable(lcp, sparse);

}
