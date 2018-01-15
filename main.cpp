#include <bits/stdc++.h>

#define itn int
#define all(x) (x).begin(), (x).end()
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())

using namespace std;

inline int nxt() {
	int x;
	scanf("%d", &x);
	return x;
}

#include "fft.h"

void test_fft() {
	auto /*function<vector<long long>(int, int)>*/ genRandVec = [](int n, int limit = mod) {
		vector<long long> result(n);
		for (auto& x : result) {
			x = rand() % limit;
		}
		return result;
	};

	auto a = genRandVec(5, 10);
	auto b = genRandVec(5, 10);

	if (multiply(a, b) == stupidMult(a, b)) {
		printf("Ok multiplication seems to work\n");
	} else {
		printf("Multiplication doesn't work properly\n");
	}

	fflush(stdout);

	for (int n : {1000, 10000, 100000, 200000, 500000}) {
		a = genRandVec(n);
		b = genRandVec(n);

		clock_t start = clock();
		auto c = multiply(a, b);
		printf("Two vectors of size %d are multiplied in %.6lf seconds\n", n, (clock() - start) / 1.0 / CLOCKS_PER_SEC);
	}
}

int main() {
	init();
	test_fft();

	return 0;
}