#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

typedef int T;
typedef long long int W;

struct Edge {
  Edge(): from(0), to(0), w(0) { }
  T from, to;
  W w;

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

ostream &operator <<(ostream &os, const Edge &edge) {
  os << edge.from + 1 << " " << edge.to + 1 << " " << edge.w;
  return os;
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

  void build_tree_on(const vector<Edge> &edges) {
    auto comp = [](const Edge *a, const Edge *b) {
        return *a < *b;
    };
    priority_queue<const Edge *, vector<const Edge *>,
                   decltype(comp)> queue(comp);
    for (auto &edge: edges)
      queue.push(&edge);

    while(tree_->edges_qty() < tree_->size() - 1) {
      auto edge = *queue.top();
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

  void add_to_tree(const Edge &edge) {
    tree_->add(edge);
    sets_.union_sets(edge.to, edge.from);
  }

};

class TreeObserver {
 public:
  TreeObserver(const Tree &tree): tree_(&tree), timer(0), k(0) {
    while ((1<<k) <= tree.size()) k++;
    up.resize(k + 1);
    for (auto &it: up)
      it.resize(k + 1);
    edges.resize(k + 1);
    for (auto &it: edges)
      it.resize(k + 1);
    in.resize(tree.size());
    out.resize(tree.size());
  }

  void traverse() {
    dfs(0);
  }

 private:

  void dfs(T v, T p = 0) {
    in[v] = ++timer;
    out[v] = ++timer;
  }

  Tree *tree_;

  vector<vector<T>> up;
  vector<vector<T>> edges;

  vector<T> in;
  vector<T> out;

  T k;
  T timer;

};

class Solver {
public:
  Solver(T vertices_qty): tree_(vertices_qty), observer_(tree_) { }

  void analyze(const vector<Edge> &edges) {
    SpanningTreeBuilder(tree_).build_tree_on(edges);
    observer_.traverse();
  }

  W weight_of_spanning_tree_with_fixed(const Edge &edge) {
    if (tree_.has(edge))
      return tree_.weight;

    return 0;
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
    cout << solver.weight_of_spanning_tree_with_fixed(edge) << endl;
}

int main() {
  solve(cin, cout);
  return 0;
}
