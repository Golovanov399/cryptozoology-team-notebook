const int L = 22;
const int N = 1 << L;
bool fft_initialized = false;

using ld = long double;
using base = complex<ld>;
using Poly = vector<ld>;

const ld pi = acosl(-1);
base angles[N + 1];
int bitrev[N];

// don't know why such eps, may be changed
const ld eps = 1e-7;

inline bool eq(ld x, ld y) {
	return abs(x - y) < eps;
}

void fft_init() {
	for (int i = 0; i <= N; ++i) {
		angles[i] = {cosl(2 * pi * i / N), sinl(2 * pi * i / N)};
	}

	for (int i = 0; i < N; ++i) {
		int x = i;
		for (int j = 0; j < L; ++j) {
			bitrev[i] = (bitrev[i] << 1) | (x & 1);
			x >>= 1;
		}
	}

	fft_initialized = true;
}

inline int revBit(int x, int len) {
	return bitrev[x] >> (L - len);
}

void fft(vector<base>& a, bool inverse = false) {
	assert(fft_initialized && "you fucking cunt just write fft_init()");
	int n = a.size();
	assert(!(n & (n - 1)));	// work only with powers of two
	int l = __builtin_ctz(n);

	for (int i = 0; i < n; ++i) {
		int j = revBit(i, l);
		if (i < j) {
			swap(a[i], a[j]);
		}
	}

	for (int len = 1; len < n; len *= 2) {
		for (int start = 0; start < n; start += 2 * len) {
			for (int i = 0; i < len; ++i) {
				base x = a[start + i], y = a[start + len + i];
				int idx = N / 2 / len * i;
				base w = y * angles[inverse ? N - idx : idx];
				a[start + i] = x + w;
				a[start + len + i] = x - w;
			}
		}
	}

	if (inverse) {
		for (auto& x : a) {
			x /= n;
		}
	}
}

Poly multiply(Poly a, Poly b) {
	int n = 1;
	while (n < (int)a.size() || n < (int)b.size()) {
		n *= 2;
	}
	vector<base> ar(n + n), br(n + n);
	for (int i = 0; i < (int)a.size(); ++i) {
		ar[i] = a[i];
	}
	for (int i = 0; i < (int)b.size(); ++i) {
		br[i] = b[i];
	}
	fft(ar);
	fft(br);
	for (int i = 0; i < n + n; ++i) {
		ar[i] = ar[i] * br[i];
	}
	fft(ar, true);
	while (!ar.empty() && eq(norm(ar.back()), 0)) {
		ar.pop_back();
	}
	a.resize(ar.size());
	for (int i = 0; i < (int)a.size(); ++i) {
		a[i] = real(ar[i]);
	}
	return a;
}
