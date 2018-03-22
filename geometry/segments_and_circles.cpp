struct point {
    ld x, y;
    
    point(ld x = 0, ld y = 0): x(x), y(y) {}
    
    point operator+(const point& p) const { return point(x + p.x, y + p.y); }
    point operator-(const point& p) const { return point(x - p.x, y - p.y); }
    
    point operator*(ld t) const { return point(x * t, y * t); }
    point operator/(ld t) const { return point(x / t, y / t); }
    
    ld vprod(const point& p) const { return x * p.y - y * p.x; }
    ld sprod(const point& p) const { return x * p.x + y * p.y; }
    
    point rot() const { return point(-y, x); }
    
    point norm() const { return *this / len(); }
    bool valid() const { return isfinite(x); }
    
    ld len() const { return hypot(x, y); }
    ld sql() const { return x * x + y * y; }
    
    int half() const {
        if (abs(y) > eps)
            return y < 0;
        else
            return x < -eps;
    }
};

point invalid(INFINITY, INFINITY);

point segmentIntersect(point a, point b, point c, point d) {
    b = b - a;
    d = d - c;
    
    if (abs(b.vprod(d)) < eps) return invalid;
    
    // a + bu = c + dv
    ld u = (c - a).vprod(d) / b.vprod(d);
    ld v = (a - c).vprod(b) / d.vprod(b);
    
    if (u >= -eps && v >= -eps && u <= 1 + eps && v <= 1 + eps)
        return a + b * u;
    
    return invalid;
}

vector<point> lineCircleIntersect(point a, point b, point c, ld r) {
    point n = (b - a).norm().rot();
    ld d = n.sprod(a - c);
    if (abs(d) > r + eps) return {};
    
    if (abs(abs(d) - r) < eps)
        return { c + n * d };
    
    ld x = sqrt(max<ld>(0, r * r - d * d));
    return { c + n * d + n.rot() * x, c + n * d - n.rot() * x };
}

vector<point> segmentCircleIntersect(point a, point b, point c, ld r) {
    auto pts = lineCircleIntersect(a, b, c, r);
    
    vector<point> ans;
    for (point& p: pts) {
        assert(abs((p - c).len() - r) < eps);
        assert(abs((p - a).vprod(b - a)) < eps);
        
        if ((p - a).sprod(p - b) <= eps)
            ans.push_back(p);
    }
    
    return ans;
}

vector<point> circleCircleIntersect(point c1, ld r1, point c2, ld r2) {
    // r_1 ^ 2 - h^2 = x^2
    // r_2 ^ 2 - h^2 = (d - x)^2 = x^2 -2dx + d^2
    // d^2 -2dx = r_2^2 - r_1^2
    
    ld d = (c2 - c1).len();
    
    if (d > r1 + r2 + eps || d < abs(r2 - r1) - eps || abs(d) < eps) return {};
    
    ld x = (d * d - r2 * r2 + r1 * r1) / (2 * d);
    point dir = (c2 - c1).norm();
    
    ld h = sqrt(max<ld>(r1 * r1 - x * x, 0));
    
    if (h < eps)
        return { c1 + dir * x };
    else
        return { c1 + dir * x + dir.rot() * h, c1 + dir * x - dir.rot() * h };
}
