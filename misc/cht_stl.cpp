const li is_query = -(1LL << 62);

struct Line {
    // mx + b
    li m, b;
    mutable function<const Line *()> succ;

    bool operator<(const Line &rhs) const {
        if (rhs.b != is_query) return m < rhs.m;
        const Line *s = succ();
        if (!s) return 0;
        li x = rhs.m;
        return b - s->b < (s->m - m) * x;
    }
};

using LI = __int128_t; // or long double; long long if line coords are <= 1e9

// WARNING: don't try to swap this structure (e.g. in lower to greater):
// it will make next iterators inconsistent and SIGSEGV
struct HullDynamic : public multiset<Line> {
    bool bad(iterator y) {
        auto z = next(y);
        if (y == begin()) {
            if (z == end()) return 0;
            return y->m == z->m && y->b <= z->b;
        }
        auto x = prev(y);
        if (z == end()) return y->m == x->m && y->b <= x->b;

        return (x->b - y->b) * (LI)(z->m - y->m) >= (y->b - z->b) * (LI)(y->m - x->m);
    }

    void insert_line(li m, li b) {
        auto y = insert({m, b});
        y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
        if (bad(y)) {
            erase(y);
            return;
        }
        while (next(y) != end() && bad(next(y))) erase(next(y));
        while (y != begin() && bad(prev(y))) erase(prev(y));
    }

    li getMax(li x) {
        auto l = *lower_bound((Line) {x, is_query});
        return l.m * x + l.b;
    }
};
