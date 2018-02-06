void zFunction(const string& s, vector<int>& z) {
	int n = s.length();
	if (n == 0)
		return;
	z[0] = 0;
	int l = 0, r = 0;
	for (int i = 1; i < n; ++i) {
		if (i <= r)
			z[i] = min(z[i - l], r - i + 1);
		while (i + z[i] < n && s[i + z[i]] == s[z[i]])
			++z[i];
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
}
