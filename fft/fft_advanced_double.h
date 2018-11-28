Poly derivative(Poly a) {
	if (a.empty()) {
		return a;
	}
	for (int i = 0; i < (int)a.size(); ++i) {
		a[i] = a[i] * i;
	}
	a.erase(a.begin());
	return a;
}

// returns b(x) = int_0^x{a(t)dt}
Poly primitive(Poly a) {
	if (a.empty()) {
		return a;
	}
	for (int i = 0; i < (int)a.size(); ++i) {
		a[i] = a[i] / (i + 1);
	}
	a.insert(a.begin(), 0);
	return a;
}

Poly add(Poly a, const Poly& b) {
	a.resize(max(a.size(), b.size()));
	for (int i = 0; i < (int)b.size(); ++i) {
		a[i] += b[i];
	}
	return a;
}

Poly sub(Poly a, const Poly& b) {
	a.resize(max(a.size(), b.size()));
	for (int i = 0; i < (int)b.size(); ++i) {
		a[i] -= b[i];
	}
	return a;
}

Poly normalize(Poly a) {
	while (!a.empty() && eq(a.back(), 0)) {
		a.pop_back();
	}
	return a;
}

// get such b that a * b = 1 mod x^{prec}
Poly getInversed(Poly a, int prec) {
	assert(!eq(a[0], 0));

	Poly res = {1.0 / a[0]};
	int k = 1;
	while (k < prec) {
		k *= 2;
		Poly tmp = multiply(res, Poly({a.begin(), a.begin() + min(k, (int)a.size())}));
		for (auto& x : tmp) {
			x = -x;
		}
		tmp[0] += 2;

		res = multiply(tmp, res);
		res.resize(k);
	}
	res.resize(prec);
	return res;
}

// get such q and r that a = b * q + r, deg(r) < deg(b)
pair<Poly, Poly> divMod(Poly a, Poly b) {
	int n = a.size();
	int m = b.size();
	if (n < m) {
		return {{0}, a};
	}
	reverse(all(a));
	reverse(all(b));
	auto quotient = multiply(a, getInversed(b, n - m + 1));
	quotient.resize(n - m + 1);
	reverse(all(a));
	reverse(all(b));
	reverse(all(quotient));
	auto remainder = sub(a, multiply(b, quotient));
	remainder = normalize(remainder);
	return {quotient, remainder};
}

// this is for multipoint and interpolate functions
vector<Poly> getSegmentProducts(const vector<ld>& pts) {
	vector<Poly> segment_polys;
	function<int(int, int)> fill_polys = [&](int l, int r) {
		if (l + 1 == r) {
			segment_polys.push_back({-pts[l], 1});
			return (int)segment_polys.size() - 1;
		}
		int m = (l + r) / 2;
		int i = fill_polys(l, m);
		int j = fill_polys(m, r);
		auto new_poly = multiply(segment_polys[i], segment_polys[j]);
		segment_polys.push_back(new_poly);
		return (int)segment_polys.size() - 1;
	};
	fill_polys(0, pts.size());

	return segment_polys;
}

// get p and {x1, x2, ..., xn}, return {p(x1), p(x2), ..., p(xn)}
vector<ld> multipoint(const Poly& poly, const vector<ld>& pts) {
	if (pts.empty()) {
		return {};
	}

	vector<Poly> segment_polys = getSegmentProducts(pts);
	vector<ld> ans;
	function<void(const Poly&)> fill_ans = [&](const Poly& p) {
		if ((int)segment_polys.back().size() <= 2) {
			ans.push_back(p.empty() ? 0 : p[0]);
			segment_polys.pop_back();
			return;
		}
		segment_polys.pop_back();
		fill_ans(divMod(p, segment_polys.back()).second);
		fill_ans(divMod(p, segment_polys.back()).second);
	};
	fill_ans(poly);
	reverse(all(ans));

	return ans;
}

// get {x1, ..., xn} and {y1, ..., yn}, return such p that p(xi) = yi
Poly interpolate(const vector<ld>& xs, const vector<ld>& ys) {
	assert(xs.size() == ys.size());
	if (xs.empty()) {
		return {0};
	}

	vector<Poly> segment_polys = getSegmentProducts(xs);
	auto der = derivative(segment_polys.back());
	auto coeffs = multipoint(der, xs);
	for (auto& c : coeffs) {
		// definitely not zero
		c = 1.0 / c;
	}
	for (int i = 0; i < (int)ys.size(); ++i) {
		coeffs[i] *= ys[i];
	}

	function<Poly()> get_ans = [&]() {
		Poly res;
		if (segment_polys.back().size() <= 2) {
			segment_polys.pop_back();
			res = {coeffs.back()};
			coeffs.pop_back();
		} else {
			segment_polys.pop_back();

			auto p1 = segment_polys.back();
			auto q1 = get_ans();

			auto p2 = segment_polys.back();
			auto q2 = get_ans();

			res = add(multiply(p1, q2), multiply(p2, q1));
		}
		return res;
	};
	return normalize(get_ans());
}

// takes c + b, returns ln(c) + b - b^2/2 + b^3/3 - ... mod x^{prec}, here b is xb'
// ofc c must be > 0
Poly logarithm(Poly a, int prec) {
	assert(a[0] > eps);
	auto c = a[0];
	for (auto& x : a) {
		x /= c;
	}
	auto res = primitive(multiply(derivative(a), getInversed(a, prec)));
	res.resize(prec);
	res[0] += logl(c);
	return res;
}

// returns 1 + a + a^2/2 + a^3/6 + ... mod x^{prec}
Poly exponent(Poly a, int prec) {
	auto c = a[0];
	a[0] = 0;

	Poly res = {1};
	int k = 1;
	while (k < prec) {
		k *= 2;
		Poly tmp = {a.begin(), a.begin() + min(k, (int)a.size())};
		tmp[0] += 1;
		tmp = sub(tmp, logarithm(res, k));

		res = multiply(tmp, res);
		res.resize(k);
	}
	res.resize(prec);
	for (auto& x : res) {
		x *= expl(c);
	}
	return res;
}
