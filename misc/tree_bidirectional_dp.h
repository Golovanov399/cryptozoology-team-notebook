/* For any commutative function f({x, y, ..., z}) = f(x, f(y, f(..., z)))
 * like sum, min, max, or, xor, and, etc
 * calculates in dp[i][j] f(subtree),
 * where subtree is a connectivity component of G \ (i, a[i][j]) with vertex a[i][j]
 */

const int N = 222222;
vector<int> a[N];
vector<int> dp[N];
int par[N];

#define data asdf
int data[N];

inline int f(int x, int y) {
	return x | y;
}

int dfsDown(int v) {
	int res = data[v];
	for (int i = 0; i < (int)a[v].size(); ++i) {
		int to = a[v][i];
		if (to == par[v]) {
			continue;
		}
		par[to] = v;
		res = f(res, dp[v][i] = dfsDown(to));
	}
	return res;
}

void dfsUp(int v, int to_parent = 0) {
	vector<int> pref, suf;
	pref.reserve(a[v].size());
	suf.reserve(a[v].size());
	int j = 0;
	for (int i = 0; i < (int)a[v].size(); ++i) {
		int to = a[v][i];
		if (to == par[v]) {
			dp[v][i] = to_parent;
			continue;
		}
		pref.push_back(j ? f(pref[j - 1], dp[v][i]) : dp[v][i]);
		++j;
	}
	j = 0;
	for (int i = (int)a[v].size() - 1; i >= 0; --i) {
		int to = a[v][i];
		if (to == par[v]) {
			continue;
		}
		suf.push_back(j ? f(dp[v][i], suf[j - 1]) : dp[v][i]);
		++j;
	}
	reverse(all(suf));

	j = 0;
	to_parent = f(to_parent, data[v]);
	for (int i = 0; i < (int)a[v].size(); ++i) {
		int to = a[v][i];
		if (to == par[v]) {
			continue;
		}
		int new_to_parent = to_parent;
		if (j > 0) {
			new_to_parent = f(pref[j - 1], new_to_parent);
		}
		if (j < (int)suf.size() - 1) {
			new_to_parent = f(new_to_parent, suf[j + 1]);
		}
		dfsUp(to, new_to_parent);
		++j;
	}
}
