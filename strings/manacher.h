array<vector<int>, 2> manacher(const string& s) {
	int n = s.length();
	array<vector<int>, 2> res;
	for (auto& v : res) {
		v.assign(n, 0);
	}
	for (int z = 0, l = 0, r = 0; z < 2; ++z, l = 0, r = 0) {
		for (int i = 0; i < n; ++i) {
			if (i < r) {
				res[z][i] = min(r - i + !z, res[z][l + r - i + !z]);
			}
			int L = i - res[z][i], R = i + res[z][i] - !z;
			while (L - 1 >= 0 && R + 1 < n && s[L - 1] == s[R + 1]) {
				++res[z][i];
				--L;
				++R;
			}
			if (R > r) {
				l = L;
				r = R;
			}
		}
	}
	return res;
}