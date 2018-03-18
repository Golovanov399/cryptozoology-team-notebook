
const int MAX_FFT_N = 1 << 19;

ld PI = acos((ld)-1.0);

ld co[MAX_FFT_N], si[MAX_FFT_N];

void precalc() {
    for (int i = 0; i < MAX_FFT_N; ++i) {
        co[i] = cos(2 * PI * i / MAX_FFT_N);
        si[i] = sin(2 * PI * i / MAX_FFT_N);
    }
}

#define double ld

using base = complex<double>;

void fft(vector<base> & a, bool invert) {
    int n = (int)a.size();

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j >= bit; bit >>= 1)
            j -= bit;
        j += bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        int dom = (invert ? -1 : 1);
        int DOM = MAX_FFT_N / len;
        for (int i = 0; i < n; i += len) {
            base w(1);
            for (int j = 0; j < len / 2; ++j) {
                w = base(co[DOM * j], si[DOM * j] * dom);
                auto u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < n; ++i) {
            a[i] /= (double)n;
        }
    }
}


vector <long long> multiply(const vector<int>& a, const vector<int>& b) {
    int n = 1;
    while (n <= a.size() || n <= b.size())
        n <<= 1;
    n <<= 1;
    vector<base> input[2];
    for (int w = 0; w < 2; ++w)
        input[w].assign(n, base(0, 0));
    for (int i = 0; i < a.size(); ++i)
        input[0][i] = a[i];
    for (int i = 0; i < b.size(); ++i)
        input[1][i] = b[i];
    for (auto& vec : input) {
        fft(vec, false);
    }
    vector <base> res(n);
    for (int i = 0; i < n; ++i)
        res[i] = input[0][i] * input[1][i];
    fft(res, true);
    vector<long long> answer(n);
    for (int i = 0; i < n; ++i) {
        answer[i] = (li)(res[i].real() + 0.5);
        answer[i] %= mod;
    }
    return answer;
}

const int shift = 15;

const int first_mod = 1 << shift;

vector<int> large_part(const vector<long long>& a) {
    vector<int> res(a.size());
    for (int i = 0; i < a.size(); ++i) {
        res[i] = a[i] >> shift;
    }
    return res;
}

vector<int> small_part(const vector<long long>& a) {
    vector<int> res(a.size());
    for (int i = 0; i < a.size(); ++i) {
        res[i] = a[i] & (first_mod - 1);
    }
    return res;
}

vector<long long> add(const vector<int>& q, const vector<int>& w) {
    auto res = q;
    for (int i = 0; i < w.size(); ++i) {
        res[i] += w[i];
    }
    return res;
}

vector<long long> multiply_large(const vector<long long>& a, const vector<long long>& b, int k) {
    vector<int> largeA = large_part(a), largeB = large_part(b);
    vector<int> smallA = small_part(a), smallB = small_part(b);
    vector<long long> large_mult = multiply(largeA, largeB);
    vector<long long> small_mult = multiply(smallA, smallB);
    vector<long long> middle_mult = multiply(add(smallA, largeA), add(smallB, largeB));

    vector<long long> result(large_mult.size());
    for (int i = 0; i < result.size(); ++i) {
        result[i] = ((large_mult[i] * first_mod) % mod * first_mod + small_mult[i] +
                     first_mod * (middle_mult[i] - large_mult[i] - small_mult[i]) % mod) % mod;
    }
    if (result.size() > k + 1) {
        result.resize(k + 1);
    }
    return result;
}
