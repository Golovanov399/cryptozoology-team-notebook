#include <bits/stdc++.h>

#define itn int
#define all(x) (x).begin(), (x).end()
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())

using namespace std;

#include "fft_double.h"
#include "fft_advanced_double.h"

bool eqPoly(const Poly& a, const Poly& b) {
	if (a.size() != b.size()) {
		return false;
	}
	for (int i = 0; i < (int)a.size(); ++i) {
		if (!eq(a[i], b[i])) {
			return false;
		}
	}
	return true;
}

void testMultiply() {
	assert(eqPoly(multiply({1, -2, 1}, {1, -3, 3, -1}), Poly({1, -5, 10, -10, 5, -1})));
	clock_t start = clock();
	int n = 1000000;
	Poly a(n), b(n);
	for (int i = 0; i < n; ++i) {
		a[i] = rand() * 100.0 / RAND_MAX;
		b[i] = rand() * 100.0 / RAND_MAX;
	}
	a = multiply(a, b);
	cout << "[multipl] n = " << n << ", time = " << (clock() - start) * 1.0 / CLOCKS_PER_SEC << " s\n";
	const int test_count = 25;
	for (int _ = 0; _ < test_count; ++_) {
		int n = rand() % 1000 + 10;
		Poly a(n);
		for (int i = 0; i < n; ++i) {
			a[i] = rand() * 100.0 / RAND_MAX;
		}
		int m = rand() % 1000 + 10;
		Poly b(m);
		for (int i = 0; i < m; ++i) {
			b[i] = rand() * 100.0 / RAND_MAX;
		}
		Poly c(n + m - 1);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				c[i + j] += a[i] * b[j];
			}
		}
		c = normalize(c);

		assert(eqPoly(c, multiply(a, b)));
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
	assert(add({-1, -4}, {8, 8}) == Poly({7, 4}));
	assert(add({2}, {8, 8}) == Poly({10, 8}));
}

void testSub() {
	assert(sub({7, 4}, {8, 8}) == Poly({-1, -4}));
	assert(sub({10, 8}, {2}) == Poly({8, 8}));
}

void testInverse() {
	assert(eqPoly(getInversed({1, 0, 0, 0}, 4), Poly({1, 0, 0, 0})));
	assert(eqPoly(getInversed({1, -1, 0, 0}, 4), Poly({1, 1, 1, 1})));
	const int test_count = 25;
	for (int _ = 0; _ < test_count; ++_) {
		int n = rand() % 100000 + 1000;
		Poly a(n);
		for (int i = 0; i < n; ++i) {
			a[i] = rand() * 100.0 / RAND_MAX;
		}
		while (eq(a[0], 0)) {
			a[0] = rand() * 100.0 / RAND_MAX;
		}

		auto tmp = multiply(getInversed(a, n), a);
		tmp.resize(n);
		tmp[0] -= 1;
		assert(eqPoly(tmp, Poly(n, 0)));
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
			a[i] = rand() * 100.0 / RAND_MAX;
		}
		for (int i = 0; i < m; ++i) {
			b[i] = rand() * 100.0 / RAND_MAX;
		}

		auto tmp = divMod(a, b);
		assert(tmp.second.size() < b.size());
		assert(a == add(tmp.second, multiply(b, tmp.first)));
	}
	cout << "[div mod] ok " << test_count << " tests" << endl;
}

int main() {
	fft_init();

	// testMultiply();
	testDerivative();
	testPrimitive();
	testAdd();
	testSub();
	// testInverse();
	// testDivMod();

	return 0;
}