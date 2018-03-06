using Matrix = vector<vector<ld>>;

const ld eps = 1e-6;

vector<ld> solveLinearSystem(Matrix a, vector<ld> b) {
	// solves Av = b
	assert(!a.empty());
	int n = a.size(), m = a[0].size();
	assert(n == (int)b.size());

	int row = 0;
	vector<int> cols(n);
	for (int col = 0; col < m; ++col) {
		if (row == n) {
			break;
		}
		if (abs(a[row][col]) < eps) {
			int i = row + 1;
			while (i < n && abs(a[i][col]) < eps) {
				++i;
			}
			if (i == n) {
				continue;
			}
			a[i].swap(a[row]);
			swap(b[i], b[row]);
		}

		for (int i = row + 1; i < n; ++i) {
			ld k = a[i][col] / a[row][col];
			for (int j = col; j < m; ++j) {
				a[i][j] -= k * a[row][j];
			}
			b[i] -= b[row] * k;
		}

		cols[row] = col;
		++row;
	}

	for (int i = row; i < n; ++i) {
		if (abs(b[i]) < eps) {
			return {};	// assert(false); throw PoshelNahuiException(); etc
		}
	}

	vector<ld> result(m);
	while (row) {
		--row;
		for (int i = cols[row] + 1; i < m; ++i) {
			b[row] -= a[row][i] * result[i];
		}
		result[cols[row]] = b[row] / a[row][cols[row]];
	}

	return result;
}
