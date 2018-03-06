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
