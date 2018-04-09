Poly multiply(const Poly& a, const Poly& b) {
.....
	for (int i = 0; i < n; ++i) {
		answer[i] = (li)(res[i].real() + 0.5);
		answer[i] %= mod;
	}
	return answer;
}

const int shift = 15;

const int first_mod = 1 << shift;

Poly large_part(const Poly& a) {
	Poly res(a.size());
	for (int i = 0; i < a.size(); ++i) {
		res[i] = a[i] >> shift;
	}
	return res;
}

Poly small_part(const Poly& a) {
	Poly res(a.size());
	for (int i = 0; i < a.size(); ++i) {
		res[i] = a[i] & (first_mod - 1);
	}
	return res;
}

Poly add(const Poly& q, const Poly& w) {
	auto res = q;
	res.resize(max(q.size(), w.size()));
	for (int i = 0; i < w.size(); ++i) {
		res[i] += w[i];
	}
	return res;
}

Poly multiply_large(const Poly& a, const Poly& b, int k) {
	Poly largeA = large_part(a), largeB = large_part(b);
	Poly smallA = small_part(a), smallB = small_part(b);
	Poly large_mult = multiply(largeA, largeB);
	Poly small_mult = multiply(smallA, smallB);
	Poly middle_mult = multiply(add(smallA, largeA), add(smallB, largeB));

	Poly result(large_mult.size());
	for (int i = 0; i < result.size(); ++i) {
		result[i] = ((large_mult[i] * first_mod) % mod * first_mod + small_mult[i] +
					 first_mod * (middle_mult[i] - large_mult[i] - small_mult[i]) % mod) % mod;
	}
	if (result.size() > k + 1) {
		result.resize(k + 1);
	}
	return result;
}
