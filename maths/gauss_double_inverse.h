using Matrix = vector<vector<ld>>;

const ld eps = 1e-6;

Matrix getInverse(Matrix a) {
	assert(!a.empty());
	int n = a.size();
	assert(n == (int)a[0].size());

	Matrix b(n, vector<ld>(n, 0));
	for (int i = 0; i < n; ++i) {
		b[i][i] = 1;
	}

	int row = 0;
	for (int col = 0; col < n; ++col) {
		if (abs(a[row][col]) < eps) {
			int i = row + 1;
			while (i < n && abs(a[i][col]) < eps) {
				++i;
			}
			if (i == n) {
				return {};	// assert(false); throw PoshelNahuiException(); etc
			}
			a[i].swap(a[row]);
			b[i].swap(b[row]);
		}

		for (int i = row + 1; i < n; ++i) {
			ld k = a[i][col] / a[row][col];
			for (int j = col; j < n; ++j) {
				a[i][j] -= k * a[row][j];
			}
			for (int j = 0; j < n; ++j) {
				b[i][j] -= k * b[row][j];
			}
		}

		++row;
	}

	for (int i = n - 1; i >= 0; --i) {
		for (int j = 0; j < i; ++j) {
			ld k = a[j][i] / a[i][i];
			for (int l = 0; l < n; ++l) {
				a[j][l] -= a[i][l] * k;
				b[j][l] -= b[i][l] * k;
			}
		}
		ld k = a[i][i];
		for (int l = 0; l < n; ++l) {
			b[i][l] /= k;
		}
		a[i][i] /= k;
	}

	return b;
}
