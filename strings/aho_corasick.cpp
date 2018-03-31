const int ALPHABET = 26;

struct state {
    array<int, ALPHABET> transition = {};
    int link = 0;
    
    bool isTerminal = false;
};

struct automaton {
    vector<state> states = { state() };
    int numStates = 1;
    
    void addString(const string& s) {
        int cur = 0;
        for (char c: s) {
            c -= 'a';
            int& to = states[cur].transition[c];
            if (to) {
                cur = to;
            }
            else {
                cur = to = states.size();
                states.push_back(state());
            }
        }
        states[cur].isTerminal = true;
    }
    
    void build() {
        deque<int> q;
        q.push_back(0);
        
        while (!q.empty()) {
            int v = q.front();
            q.pop_front();
            states[v].isTerminal = states[v].isTerminal || states[states[v].link].isTerminal;
            
            for (int c = 0; c < ALPHABET; ++c) {
                if (int u = states[v].transition[c]) {
                    states[u].link = v ? states[states[v].link].transition[c] : 0;
                    q.push_back(u);
                }
                else {
                    states[v].transition[c] = states[states[v].link].transition[c];
                }
            }
        }
    }
};
