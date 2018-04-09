inline int inv(int a, int b) {
	return a == 1 ? 1 : b - 1ll * inv(b % a, a) * b / a % b;
}

pair<int, int> euc(int a, int b) {
	// returns {x, y} s.t. ax + by = g
	int g = __gcd(a, b);
	a /= g, b /= g;
	int x = inv(a, b);
	int y = (1 - 1ll * a * x) / b;

	return {x, y};
}

// be careful if the whole base is long long
pair<int, int> crt(const vector<int>& mods, vector<int>& rems) {
	int rem = 0, mod = 1;
	for (int i = 0; i < (int)mods.size(); ++i) {
		long long g = __gcd(mods[i], mod);
		if (rem % g != rems[i] % g) {
			return {-1, -1};
		}

		int k = euc(mod, mods[i]).first * 1ll * (rems[i] - rem + mods[i]) % mods[i];
		if (k < 0) {
			k += mods[i];
		}
		rem += mod / g * k;
		mod = mod / g * mods[i];
	}
	return {rem, mod};
}