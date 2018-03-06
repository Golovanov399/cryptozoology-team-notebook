const int N = 100;
using Bs = bitset<N>;
using Matrix = vector<Bs>;

Bs solveLinearSystem(Matrix a, Bs b) {
	// solves Av = b
	assert(!a.empty());
	int n = a.size();

	int row = 0;
	vector<int> cols(n);
	for (int col = 0; col < N; ++col) {
		if (row == n) {
			break;
		}
		if (!a[row][col]) {
			int i = row + 1;
			while (i < n && !a[i][col]) {
				++i;
			}
			if (i == n) {
				continue;
			}
			swap(a[i], a[row]);
			b[i] = b[i] ^ b[row];
			b[row] = b[row] ^ b[i];
			b[i] = b[i] ^ b[row];
		}

		for (int i = row + 1; i < n; ++i) {
			if (a[i][col]) {
				a[i] ^= a[row];
				b[i] = b[i] ^ b[row];
			}
		}

		cols[row] = col;
		++row;
	}

	for (int i = row; i < n; ++i) {
		if (b[i]) {
			return {};	// assert(false); throw PoshelNahuiException(); etc
		}
	}

	Bs result = {};
	while (row) {
		--row;
		for (int i = cols[row] + 1; i < N; ++i) {
			b[row] = b[row] ^ (a[row][i] * result[i]);
		}
		result[cols[row]] = b[row];
	}

	return result;
}
