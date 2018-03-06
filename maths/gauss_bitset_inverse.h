const int N = 100;
using Bs = bitset<N>;
using Matrix = vector<Bs>;

Matrix getInverse(Matrix a) {
	assert(!a.empty());
	int n = a.size();

	Matrix b(n);
	for (int i = 0; i < n; ++i) {
		b[i][i] = 1;
	}

	int row = 0;
	for (int col = 0; col < n; ++col) {
		if (!a[row][col]) {
			int i = row + 1;
			while (i < n && !a[i][col]) {
				++i;
			}
			if (i == n) {
				return {};	// assert(false); throw PoshelNahuiException(); etc
			}
			swap(a[i], a[row]);
			swap(b[i], b[row]);
		}

		for (int i = row + 1; i < n; ++i) {
			if (a[i][col]) {
				a[i] ^= a[row];
				b[i] ^= b[row];
			}
		}

		++row;
	}

	for (int i = n - 1; i >= 0; --i) {
		for (int j = 0; j < i; ++j) {
			if (a[j][i]) {
				a[j] ^= a[i];
				b[j] ^= b[i];
			}
		}
	}

	return b;
}
