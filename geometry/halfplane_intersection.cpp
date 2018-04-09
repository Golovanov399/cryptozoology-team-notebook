using ld = double;
const ld eps = 1e-9;

struct point {
    ld x, y;
    
    point(ld x = 0, ld y = 0): x(x), y(y) {}
    
    point operator+(const point& p) const { return point(x + p.x, y + p.y); }
    point operator-(const point& p) const { return point(x - p.x, y - p.y); }
    
    point operator*(ld t) const { return point(x * t, y * t); }
    point operator/(ld t) const { return point(x / t, y / t); }
    
    point rot() const { return point(-y, x); }
    
    ld vprod(const point& p) const { return x * p.y - y * p.x; }
    ld sprod(const point& p) const { return x * p.x + y * p.y; }
    
    int half() const {
        if (y)
            return y < -eps;
        else
            return x < -eps;
    }
    
    ld sql() const { return x * x + y * y; }
    ld len() const { return sqrt(sql()); }
    
    bool operator<(const point& p) const { return make_pair(x, y) < make_pair(p.x, p.y); }
};

int sign(ld x) {
    return abs(x) > eps ? (x > 0 ? 1 : -1) : 0;
}

int vecLess(const point& a, const point& b) {
    if (a.half() != b.half())
        return a.half() < b.half() ? 1 : -1;
    else {
        return sign(a.vprod(b));
    }
}

struct halfplane {
    // ax + by + c >= 0
    ld a, b, c;
    int type;
    
    tuple<ld, ld, ld> get() const { return make_tuple(a, b, c); }
    bool operator<(const halfplane& rhs) const { return get() < rhs.get(); }
    
    point norm() const { return point(a, b); }
    
    point intersect(const halfplane& h) const {
        ld x = -c * h.b + b * h.c;
        ld y = a * -h.c + c * h.a;
        ld denum = a * h.b - b * h.a;
        return point(x / denum, y / denum);
    }
};

// does intersection of a and c belong to b?
// assumes that a.vprod(c) > 0!
bool interAccepted(const halfplane& a, const halfplane& b, const halfplane& c) {
    // Determinant of 3x3 matrix formed by a, b, c
    return a.a * (b.b * c.c - b.c * c.b) - a.b * (b.a * c.c - b.c * c.a) + a.c * (b.a * c.b - b.b * c.a) < 0;
}

void sanitizeHalfplanes(vector<halfplane>& planes, bool doAdd, bool doSort) {
    // Add bouding box
    const ld INF = 1e9;
    if (doAdd) {
        planes.push_back(halfplane { 1, 0, INF });
        planes.push_back(halfplane { -1, 0, INF });
        planes.push_back(halfplane { 0, 1, INF });
        planes.push_back(halfplane { 0, -1, INF });
    }
    
    // Normalize halfplanes. This is used when selecting strictest of parallel halfplanes
    // NOT NEEDED if there are no collinear (and not antiparallel) normals, but may improve precision
    for (halfplane& h: planes) {
        ld len = h.norm().len();
        h.a /= len;
        h.b /= len;
        h.c /= len;
    }
    
    if (doSort)
        sort(all(planes), [&](halfplane& a, halfplane& b) { return vecLess(a.norm(), b.norm()) > 0; });
}

class polygon {
public:
    vector<point> pts;
    
    polygon(const vector<point>& pts = vector<point>()): pts(pts) {}

    ld getDoubleSquare() const {
        ld result = 0;
        int n = pts.size();
        for (int i = 1; i < n - 1; ++i) {
            result += (pts[i] - pts[0]).vprod(pts[i + 1] - pts[0]);
        }
        return abs(result);
    }
};

// Returns halfplane through points a and b,
// inner part is counter-clockwise from a->b segment
halfplane byPoints(point a, point b) {
    // rot counter clockwise, n points to area inside halfplane intersection
    point n = (b - a).rot();
    return halfplane { n.x, n.y, -n.sprod(a) };
}

// empty return polygon/vector denotes empty intersection
// degenerate intersections are reported as empty

// CALL sanitizeHalfplanes WITH SORT AND/OR ADD BOUNDING BOX BEFORE USING!
polygon getPolygon(const vector<halfplane>& planes) {
    int l = 0, r = 0;
    static vector<halfplane> ans;
    ans.clear();
    ans.reserve(planes.size());
    
    for (int L = 0; L < planes.size();) {
        int R = L + 1;
        while (R < planes.size() && abs(planes[L].norm().vprod(planes[R].norm())) < eps) ++R;
        
        // choose most powerful inequality among those with equal normals
        // assumes that normals are identity!
        const halfplane& h = *min_element(planes.begin() + L, planes.begin() + R, [](const halfplane& a, const halfplane& b) { return a.c < b.c; });
        L = R;
    
        while (r - l > 1 && !interAccepted(ans[r - 2], h, ans[r - 1])) {
            ans.pop_back();
            --r;
        }
        
        while (r - l > 1 && !interAccepted(ans[l], h, ans[l + 1])) {
            ++l;
        }
        
        // WATCH OUT: you may need to tweak eps here for severe problems
        if (r - l > 0 && ans[r - 1].norm().vprod(h.norm()) <= -1e-7) {
            return polygon();
        }
        
        if (r - l < 2 || interAccepted(ans[r - 1], ans[l], h)) {
            ans.push_back(h);
            r++;
        }
    }
    
    assert(r == ans.size());
    
    // IF YOU NEED HALFPLANES:
    // return vector<halfplane>(ans.begin() + l, ans.end());
    
    int n = r - l;
    
    polygon poly;
    poly.pts.reserve(n);
    for (int i = 0; i < n; ++i) {
        poly.pts.push_back(ans[l + i].intersect(ans[l + (i + 1) % n]));
    }
    
    return poly;
}
