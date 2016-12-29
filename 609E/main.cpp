#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

typedef int T;
typedef long long int W;

struct Edge {
  Edge(): from(0), to(0), w(0), in_tree(false) { }
  T from, to;
  W w;
  bool in_tree;

  bool operator<(const Edge &rhs) const {
    return w > rhs.w;
  }
};

istream &operator >>(istream &is, Edge &edge) {
  T from, to;
  is >> from >> to >> edge.w;
  edge.to = to - 1;
  edge.from = from - 1;
  return is;
}

struct DisjointSets {
  DisjointSets(size_t size): parents(size), rank(size, 0) {
    for (int i = 0; i < size; ++i)
      parents[i] = i;
  }

  int find_set(int i) {
    if (parents[i] == i)
      return i;
    return parents[i] = find_set(parents[i]);
  }

  bool not_from_one_set(int x, int y) {
    return !(find_set(x) == find_set(y));
  }

  void union_sets(int x, int y) {
    int a = find_set(x);
    int b = find_set(y);
    if (a == b)
      return;

    if (rank[a] < rank[b])
      parents[a] = b;
    else {
      parents[b] = a;
      if (rank[a] == rank[b])
        rank[a]++;
    }
  }

  vector<T> parents;
  vector<T> rank;
};

struct Relation {
  Relation(): to(-1), w(-1) { }
  Relation(T to, W w): to(to), w(w) { }

  T to;
  W w;
};

struct Tree {
  Tree(size_t size): vertices(size), weight(0), edges_qty_(0) { }

  void add(const Edge &e) {
    vertices[e.from].push_back(Relation(e.to, e.w));
    vertices[e.to].push_back(Relation(e.from, e.w));
    weight += e.w;
    edges_qty_++;
  }

  inline size_t edges_qty() const {
    return edges_qty_;
  }

  bool has(const Edge &e) const {
    for (auto &relation: vertices[e.from])
      if (relation.to == e.to)
        return true;
    return false;
  }

  const vector<Relation> &relations_for(T vertex) const {
    return vertices[vertex];
  }

  inline size_t size() const {
    return vertices.size();
  }

  W weight;
  size_t edges_qty_;
  vector<vector<Relation>> vertices;
};

class SpanningTreeBuilder {
 public:

  SpanningTreeBuilder(Tree &tree): tree_(&tree), sets_(tree.size()) {}

  void build_tree_on(vector<Edge> &edges) {
    auto comp = [](const Edge *a, const Edge *b) {
        return *a < *b;
    };
    priority_queue<Edge *, vector<Edge *>,
                   decltype(comp)> queue(comp);
    for (auto &edge: edges)
      queue.push(&edge);

    while(tree_->edges_qty() < tree_->size() - 1) {
      Edge &edge = *queue.top();
      queue.pop();
      if (is_valid_to_add(edge))
        add_to_tree(edge);
    }
  }

 private:
  Tree *tree_;
  DisjointSets sets_;

  bool is_valid_to_add(const Edge &e) {
    return sets_.not_from_one_set(e.to, e.from);
  }

  void add_to_tree(Edge &edge) {
    tree_->add(edge);
    edge.in_tree = true;
    sets_.union_sets(edge.to, edge.from);
  }

};

class TreeObserver {
 public:
  TreeObserver(const Tree &tree): tree_(&tree), timer(0), k(0) {
    while ((1<<k) <= tree.size()) k++;
    T n = tree.size();
    up.resize(n);
    for (auto &it: up)
      it.resize(k + 1);
    ws.resize(n);
    for (auto &it: ws)
      it.resize(k + 1);
    in.resize(n);
    out.resize(n);
  }

  void traverse() {
    Relation initial_relation(0, 0);
    dfs(initial_relation);
  }

  T lowest_ancestor(T u, T v) const {
    if (upper(u, v)) return u;
    if (upper(v, u)) return v;
    for (int i = k; i >= 0; --i) {
      T candidate = up[u][i];
      if (!upper(candidate, v))
        u = candidate;
    }
    return up[u][0];
  }

  W max_weight(T ancestor, T u) const {
    assert(upper(ancestor, u));
    W weight = 0;
    for (int i = k; i >= 0; --i) {
      T candidate = up[u][i];
      if (!upper(candidate, ancestor)) {
        weight = max(weight, ws[u][i]);
        u = candidate;
      }
    }
    return max(weight, ws[u][0]);
  }

  bool upper(T u, T v) const {
    return in[u] <= in[v] && out[u] >= out[v];
  }

 private:

  void dfs(Relation relation, T p = 0) {
    T v = relation.to;
    W w = relation.w;

    in[v] = ++timer;

    up[v][0] = p;
    ws[v][0] = w;
    for (T i = 1; i <= k; ++i) {
      T ancestor = up[v][i - 1];
      up[v][i] = up[ancestor][i - 1];
      ws[v][i] = max(ws[ancestor][i - 1], ws[v][i - 1]);
    }

    for (auto &r: tree_->relations_for(v)) {
      T neighbour = r.to;
      if (neighbour != p) dfs(Relation(neighbour, r.w), v);
    }

    out[v] = ++timer;
  }

  const Tree *tree_;

  vector<vector<T>> up;
  vector<vector<W>> ws;

  vector<T> in;
  vector<T> out;

  T k;
  T timer;

};

class Solver {
public:
  Solver(T vertices_qty): tree_(vertices_qty), observer_(tree_) { }

  void analyze(vector<Edge> &edges) {
    SpanningTreeBuilder(tree_).build_tree_on(edges);
    observer_.traverse();
  }

  W weight_of_spanning_tree_with_fixed(const Edge &edge) const {
    if (edge.in_tree)
      return tree_.weight;

    T from = edge.from;
    T to = edge.to;

    W weight;

    T ancestor = observer_.lowest_ancestor(to, from);
    if (ancestor == to || ancestor == from) {
      T u = (ancestor == to) ? from : to;
      weight = observer_.max_weight(ancestor, u);
    } else
    weight = max(observer_.max_weight(ancestor, from),
                 observer_.max_weight(ancestor, to));
    return tree_.weight - weight + edge.w;
  }

private:

  Tree tree_;
  TreeObserver observer_;

};

void solve(istream &is, ostream &os) {
  T vertices_qty = 0, edges_qty = 0;
  is >> vertices_qty >> edges_qty;
  Solver solver(vertices_qty);

  vector<Edge> edges(edges_qty);
  for (auto &edge: edges)
    is >> edge;

  solver.analyze(edges);
  for (auto &edge: edges)
    os << solver.weight_of_spanning_tree_with_fixed(edge) << '\n';
}

int main() {
  solve(cin, cout);
  return 0;
}
