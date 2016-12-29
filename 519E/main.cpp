#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;

typedef int T;

vector<vector<T>> g;
vector<vector<T>> up;
vector<T> qtys;
vector<T> tin, tout;
int timer;
int k, n;

void dfs(T v = 0, T p = 0) {
  T qty = 0;
  tin[v] = ++timer;
  up[v][0] = p;
  for (int i = 1; i <= k; ++i)
    up[v][i] = up[up[v][i - 1]][i - 1];
  for (int i = 0; i < g[v].size(); ++i) {
    int to = g[v][i];
    if (p != to) {
      dfs(to, v);
      qty += qtys[to];
    }
  }
  tout[v] = ++timer;
  qtys[v] = qty + 1;
}

bool upper (int a, int b) {
  return tin[a] <= tin[b] && tout[a] >= tout[b];
}

T lca(T u, T v) {
  if (upper(v, u)) return v;
  if (upper(u, v)) return u;
  for (int i = k; i >= 0; --i) {
    if (!upper(up[u][i], v)) {
      u = up[u][i];
    }
  }
  return up[u][0];
}

T th_ancestor(T u, T d) {
  for (int i = k; i >= 0 && d > 0; --i) {
    if (pow(2, i) <= d) {
      d -= pow(2, i);
      u = up[u][i];
      assert(d >= 0);
    }
  }
  return u;
}

int distance(T v, T u) {
  if (u == v) return 0;
  assert(upper(v, u));
  size_t distance = 0;
  for (int i = k; i >= 0; --i) {
    if (!upper(up[u][i], v)) {
      u = up[u][i];
      distance += pow(2, i);
    }
  }
  return distance + 1;
}

int main() {
  timer = 0;
  cin >> n;
  k = 1;
  while ((1 << k) <= n)  ++k;

  g.resize(n);
  up.resize(n);
  tin.resize(n);
  tout.resize(n);
  qtys.resize(n);
  timer = 0;

  for (auto &it: up)
    it.resize(k + 1);

  int u, v;
  for (int i = 0; i < n - 1; ++i) {
    cin >> u >> v;
    u -= 1;
    v -= 1;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs();

  int m = 0;
  cin >> m;
  bool is_even;
  T ancestor;

  vector<pair<T, T>> queries(m);

  for (auto &query: queries)
    cin >> query.first >> query.second;
  T qty = 0;
  for (auto &query: queries) {
    u = query.first;
    v = query.second;
    u -= 1;
    v -= 1;

    ancestor = lca(u, v);

    cout << "ancestor " << ancestor + 1 << endl;

    if (ancestor == u || ancestor == v) {
      u = upper(u, v) ? v : u;
      T d = distance(ancestor, u);
      if (d % 2 != 0) {
        cout << 0 << '\n';
        continue;
      }
//      cout << "distance : " << d << endl;
      T median = th_ancestor(u, d/2);
//      cout << "u : " << u + 1 << endl;
//      cout << "median : " << median + 1 << endl;
      T qty = 1;
      for (auto neighbour: g[median]) {
        if (!upper(neighbour, u))
          qty += qtys[neighbour];
      }
      cout << qty << '\n';
    } else {
      T qty = n;
      for (auto neighbour: g[ancestor]) {
        if ((upper(neighbour, u) || upper(neighbour, v))
            && !upper(neighbour, ancestor)) {
          cout << "cutting of subtree : " << neighbour + 1 << endl;
          qty -= qtys[neighbour];
        }
      }
      cout << qty << '\n';
    }
  }
  return 0;
}
