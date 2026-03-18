#pragma once
#include <vector>
struct DSU {
  int n;
  std::vector<int> p;
  std::vector<int> r;
  int num_sets;
  DSU(int n) : n(n), p(n), r(n), num_sets(n) {
    for (int i = 0; i < n; i++)
      p[i] = i;
    for (int i = 0; i < n; i++)
      r[i] = 0;
  }
  int find(int x) {
    if (p[x] != x)
      p[x] = find(p[x]);
    return p[x];
  }
  int same_set(int x, int y) { return find(x) == find(y); }
  void union_sets(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y)
      return;
    if (r[x] < r[y]) {
      p[x] = y;
    } else if (r[x] > r[y]) {
      p[y] = x;
    } else {
      p[x] = y;
      r[y]++;
    }
    num_sets--;
  }
};
