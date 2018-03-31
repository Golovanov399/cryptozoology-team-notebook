const int INF = (int)1e6;

struct Line {
  int k;
  li b;
  bool operator < (const Line& ot) const {
    if (k != ot.k) {
      return k > ot.k;
    }
    return b < ot.b;
  }
  li eval(li x) {
    return k * 1LL * x + b;
  }
};

double get_intersect(Line& q, Line& w) {
  return (q.b - w.b) / 1.0 / (w.k - q.k);
}

struct Hull {
  vector<Line> lines;
  vector<double> borders;
  int Size = 0;
  void append(Line cur) {
    lines.push_back(cur);
  }
  void set_size(int val) {
    Size = val;
  }
  void build() {
    sort(all(lines));
    borders.clear();
    vector<Line> new_lines;
    for (auto& line : lines) {
      if (!new_lines.empty() && new_lines.back().k == line.k) {
        continue;
      }
      while (new_lines.size() > 1 && get_intersect(new_lines[new_lines.size() - 2], new_lines.back()) > get_intersect(new_lines.back(), line)) {
        new_lines.pop_back();
        borders.pop_back();
      }
      if (new_lines.empty()) {
        borders.push_back(-INF);
      } else {
        borders.push_back(get_intersect(new_lines.back(), line));
      }
      new_lines.push_back(line);
    }
    new_lines.swap(lines);
  }
  int size() {
    return Size;
  }
  li get_min(li x) {
    int id = (int)(lower_bound(all(borders), (double)x) - borders.begin());
    li res = (li)1e18;
    for (int i = max(id - 1, 0); i < min(id + 2, (int)lines.size()); ++i) {
      res = min(res, lines[i].eval(x));
    }
    return res;
  }
};

struct Lupa {
  vector<Hull> hulls;
  int Size = 0;
  void append_line(Line cur) {
    hulls.push_back(Hull());
    hulls.back().append(cur);
    hulls.back().set_size(1);
    while (hulls.size() >= 2 && hulls.back().size() == hulls[hulls.size() - 2].size()) {
      for (auto& item : hulls.back().lines) {
        hulls[hulls.size() - 2].append(item);
      }
      hulls.pop_back();
      hulls.back().set_size(hulls.back().size() * 2);
    }
    hulls.back().build();
    ++Size;
  }
  li get_min(li x) {
    li res = (li)1e18;
    for (auto& vec : hulls) {
      res = min(res, vec.get_min(x));
    }
    return res;
  }
  int size() {
    return Size;
  }
  void merge_with(Lupa& ot) {
    for (auto& vec : ot.hulls) {
      for (auto& item : vec.lines) {
        append_line(item);
      }
      vec.lines.clear();
    }
  }
  void make_swap(Lupa& ot) {
    swap(ot.Size, Size);
    ot.hulls.swap(hulls);
  }
};
