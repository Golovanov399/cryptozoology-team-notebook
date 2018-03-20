#include <bits/stdc++.h>

#define itn int
#define all(x) (x).begin(), (x).end()
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())

using namespace std;

#include "fft_integer.h"
#include "fft_advanced_integer.h"

void testMultiply() {
	assert(multiply({1, mod - 2, 1}, {1, mod - 3, 3, mod - 1}) == Poly({1, mod - 5, 10, mod - 10, 5, mod - 1}));
	clock_t start = clock();
	int n = 1000000;
	Poly a(n), b(n);
	for (int i = 0; i < n; ++i) {
		a[i] = rand() % mod;
		b[i] = rand() % mod;
	}
	a = multiply(a, b);
	cout << "[multipl] n = " << n << ", time = " << (clock() - start) * 1.0 / CLOCKS_PER_SEC << " s\n";
	const int test_count = 25;
	for (int _ = 0; _ < test_count; ++_) {
		int n = rand() % 100 + 10;
		Poly a(n);
		for (int i = 0; i < n; ++i) {
			a[i] = rand() % mod;
		}
		int m = rand() % 100 + 10;
		Poly b(m);
		for (int i = 0; i < m; ++i) {
			b[i] = rand() % mod;
		}
		Poly c(n + m - 1);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				c[i + j] += a[i] * b[j] % mod;
			}
		}
		for (auto& x : c) {
			x %= mod;
		}
		c = normalize(c);
		assert(c == multiply(a, b));
	}
	cout << "[multipl] ok " << test_count << " tests" << endl;
}

void testDerivative() {
	assert(derivative({}) == Poly({}));
	assert(derivative({1}) == Poly({}));
	assert(derivative({1, 2, 3, 4, 5}) == Poly({2, 6, 12, 20}));
}

void testPrimitive() {
	assert(primitive({}) == Poly({}));
	assert(primitive({2, 6, 12, 20}) == Poly({0, 2, 3, 4, 5}));
}

void testAdd() {
	assert(add({mod - 1, mod - 4}, {8, 8}) == Poly({7, 4}));
	assert(add({2}, {8, 8}) == Poly({10, 8}));
}

void testSub() {
	assert(sub({7, 4}, {8, 8}) == Poly({mod - 1, mod - 4}));
	assert(sub({10, 8}, {2}) == Poly({8, 8}));
}

void testInverse() {
	const int test_count = 25;
	for (int _ = 0; _ < test_count; ++_) {
		int n = rand() % 100000 + 1000;
		Poly a(n);
		for (int i = 0; i < n; ++i) {
			a[i] = rand() % mod;
		}
		while (!a[0]) {
			a[0] = rand() % mod;
		}

		auto tmp = multiply(getInversed(a, n), a);
		tmp.resize(n);
		tmp[0] -= 1;
		assert(tmp == Poly(n, 0));
	}
	cout << "[inverse] ok " << test_count << " tests" << endl;
}

void testDivMod() {
	const int test_count = 25;
	for (int _ = 0; _ < test_count; ++_) {
		int n = rand() % 100000 + 1000;
		Poly a(n);
		int m = rand() % (int)(1.1 * n) + 10;
		Poly b(m);
		for (int i = 0; i < n; ++i) {
			a[i] = rand() % mod;
		}
		for (int i = 0; i < m; ++i) {
			b[i] = rand() % mod;
		}

		auto tmp = divMod(a, b);
		assert(tmp.second.size() < b.size());
		assert(a == add(tmp.second, multiply(b, tmp.first)));
	}
	cout << "[div mod] ok " << test_count << " tests" << endl;
}

#define apply huepply

long long apply(const Poly& a, long long x) {
	long long res = 0;
	long long cur = 1;
	for (auto c : a) {
		res += c * cur % mod;
		cur = cur * x % mod;
	}
	return res % mod;
}

void testMultipoint() {
	const int test_count = 25;
	for (int _ = 0; _ < test_count; ++_) {
		int n = rand() % 1000 + 10;
		Poly p(n);
		for (int i = 0; i < n; ++i) {
			p[i] = rand() % mod;
		}
		int m = rand() % 1000 + 100;
		vector<long long> x(m);
		for (int i = 0; i < m; ++i) {
			x[i] = rand() % mod;
		}

		auto y = multipoint(p, x);
		for (int i = 0; i < m; ++i) {
			assert(apply(p, x[i]) == y[i]);
		}
	}
	cout << "[multipt] ok " << test_count << " tests" << endl;
}

void testInterpolation() {
	const int test_count = 25;
	for (int _ = 0; _ < test_count; ++_) {
		int n = rand() % 1000 + 10;
		vector<long long> x(n), y(n);
		set<long long> was;
		for (int i = 0; i < n; ++i) {
			do {
				x[i] = rand() % mod;
			} while (was.count(x[i]));
			was.insert(x[i]);
			y[i] = rand() % mod;
		}

		auto p = interpolate(x, y);
		assert((int)p.size() <= n);
		for (int i = 0; i < n; ++i) {
			assert(apply(p, x[i]) == y[i]);
		}
	}
	cout << "[intrplt] ok " << test_count << " tests" << endl;
}

int main() {
	fft_init();

	testMultiply();
	testDerivative();
	testPrimitive();
	testAdd();
	testSub();
	testInverse();
	testDivMod();
	testMultipoint();
	testInterpolation();

	return 0;
}