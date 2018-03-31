struct Graph {
    void read() {
        int m;
        cin >> n >> m;
        
        e.resize(n);
        
        for (int i = 0; i < m; ++i) {
            int u, v;
            cin >> u >> v;
            --u; --v;
            e[u].push_back(v);
            e[v].push_back(u);
        }
    }
    
    /* COMMON PART */
    
    int n;
    vector<vector<int>> e;
    
    int counter = 1;
    vector<int> inTime, minInTime;

    void dfs(int v, int p = -1) {
        minInTime[v] = inTime[v] = counter++;

        for (int u: e[v]) {
            if (u == p) continue;

            if (!inTime[u]) {
                dfs(u, v);
                minInTime[v] = min(minInTime[v], minInTime[u]);
            }
            else {
                minInTime[v] = min(minInTime[v], inTime[u]);
            }
        }
    }
    
    vector<char> used;
    
    /* COMPONENTS SEPARATED BY BRIDGES (COLORING) */
    
    int nColors;
    vector<int> color;
    
    void colorDfs(int v, int curColor) {
        color[v] = curColor;
        
        for (int u: e[v]) {
            if (color[u] != -1) continue;
            
            colorDfs(u, minInTime[u] > inTime[v] ? nColors++ : curColor);
        }
    }
    
    void findVertexComponents() {
        inTime.assign(n, 0);
        minInTime.assign(n, 0);
        counter = 1;

        for (int i = 0; i < n; ++i)
            if (!inTime[i])
                dfs(i);

        nColors = 0;
        color.assign(n, -1);
        for (int i = 0; i < n; ++i)
            if (color[i] == -1) {
                colorDfs(i, nColors++);
            }
    }

    /* COMPONENTS SEPARATED BY JOINTS (EDGE COMPONENTS) */
    
    struct Edge {
        int u, v;
    };
    
    // Cactus loops can be parsed as .u of every edge
    vector<vector<Edge>> edgeComps;
    
    vector<int> colorStack;
    
    void edgeCompDfs(int v, int p = -1) {
        used[v] = true;
        
        for (int u: e[v]) {
            if (used[u]) {
                if (inTime[u] < inTime[v] && u != p) {
                    // NOTE:  && u != p makes one-edge components contain exactly one edge;
                    // if you need them as two-edge loops, remove this part of if condition
                    edgeComps[colorStack.back()].push_back({v, u});
                }
                
                continue;
            }

            bool newComp = minInTime[u] >= inTime[v];
            
            if (newComp) {
                colorStack.push_back(edgeComps.size());
                edgeComps.emplace_back();
            }
            
            edgeComps[colorStack.back()].push_back({v, u});
            edgeCompDfs(u, v);

            if (newComp) {
                colorStack.pop_back();
            }
        }
    }
    
    void findEdgeComponents() {
        inTime.assign(n, 0);
        minInTime.assign(n, 0);
        counter = 1;

        for (int i = 0; i < n; ++i)
            if (!inTime[i])
                dfs(i);

        used.assign(n, false);
        colorStack.clear();
        edgeComps.clear();
        for (int i = 0; i < n; ++i)
            if (!used[i]) {
                assert(colorStack.empty());
                edgeCompDfs(i);
            }
    }
};

