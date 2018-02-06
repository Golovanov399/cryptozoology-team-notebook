const int UNDEFINED_VALUE = -1;

class SuffixAutomaton {
public:
	struct State {
		map<char, int> transitions;
		int link;
		int maxLen;
		int firstPos, lastPos;
		int cnt;
		State():link(UNDEFINED_VALUE), firstPos(UNDEFINED_VALUE), lastPos(UNDEFINED_VALUE), maxLen(0), cnt(0) {}
	};
	vector<State> states;
	int lastState;
	SuffixAutomaton(const string& s) {
		states.push_back(State());
		lastState = 0;
		for (int i = 0; i < s.length(); ++i)
			append(s[i]);
		vector<pair<int, int>> p(states.size());
		for (int i = 0; i < p.size(); ++i) {
			p[i].second = i;
			p[i].first = states[i].maxLen;
		}
		sort(all(p));
		reverse(all(p));
		for (int i = 0; i < p.size(); ++i) {
			int curState = p[i].second;
			if (states[curState].lastPos == UNDEFINED_VALUE)
				states[curState].lastPos = states[curState].firstPos;
			if (states[curState].link != UNDEFINED_VALUE) {
				states[states[curState].link].lastPos = max(states[states[curState].link].lastPos, states[curState].lastPos);
				states[states[curState].link].cnt += states[curState].cnt;
			}
		}

	}

private:
	void append(char c) {
		int curState = states.size();
		states.push_back(State());
		states[curState].maxLen = states[lastState].maxLen + 1;
		states[curState].firstPos = states[lastState].maxLen;
		states[curState].cnt = 1;
		int prevState = lastState;
		for (; prevState != UNDEFINED_VALUE; prevState = states[prevState].link) {
			if (states[prevState].transitions.count(c))
				break;
			states[prevState].transitions[c] = curState;
		}

		if (prevState == UNDEFINED_VALUE) {
			states[curState].link = 0;
		}
		else {
			int nextState = states[prevState].transitions[c];
			if (states[nextState].maxLen == states[prevState].maxLen + 1) {
				states[curState].link = nextState;
			}
			else {
				int cloneState = states.size();
				states.push_back(State());
				states[cloneState].maxLen = states[prevState].maxLen + 1;
				states[cloneState].link = states[nextState].link;
				states[cloneState].firstPos = states[nextState].firstPos;
				states[curState].link = states[nextState].link = cloneState;

				states[cloneState].transitions = states[nextState].transitions;
				for (; prevState != UNDEFINED_VALUE && states[prevState].transitions[c] == nextState; prevState = states[prevState].link)
					states[prevState].transitions[c] = cloneState;
			}
		}
		lastState = curState;
	}

};
