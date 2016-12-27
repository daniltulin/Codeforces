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

  size_t edges_qty() const {
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

  size_t size() const {
    return vertices.size();
  }

  W weight;
  size_t edges_qty_;
  vector<vector<Relation>> vertices;
};

class Solver {
public:
  Solver(T vertices_qty): tree(vertices_qty), sets(vertices_qty),
                          timer(0), in(vertices_qty), out(vertices_qty) {
    k = log(vertices_qty) + 2;
    up.resize(vertices_qty, vector<T>(k + 1));
    ws.resize(vertices_qty, vector<W>(k + 1));
  }

  void build_spanning_tree_on(const vector<Edge> &edges) {
    auto comp = [](const Edge *a, const Edge *b) {
        return *a < *b;
    };
    priority_queue<const Edge *, vector<const Edge *>,
                   decltype(comp)> queue(comp);
    for (auto &edge: edges)
      queue.push(&edge);

    while(tree.edges_qty() < tree.size() - 1) {
      auto edge = *queue.top();
      queue.pop();
      if (is_valid_to_add(edge))
        add_to_tree(edge);
    }
  }

  W weight_of_spanning_tree_with_fixed(const Edge &edge) {
    if (tree.has(edge))
      return tree.weight;

    W weight = max_weight(edge.from, edge.to);
    return tree.weight + edge.w - weight;
  }

  void traverse_tree(T parent = 0, const Relation &relation = Relation(0, 0)) {
    T vertex = relation.to;
    in[vertex] = timer++;

    up[vertex][0] = parent;
    ws[vertex][0] = relation.w;

    for (int i = 1; i <= k; ++i) {
      up[vertex][i] = up[up[vertex][i - 1]][i - 1];
      ws[vertex][i] = max(ws[vertex][i - 1],
                          ws[up[vertex][i - 1]][i - 1]);
    }

    for (auto &relation: tree.relations_for(vertex)) {
      T neighbour = relation.to;
      if (parent != neighbour)
        traverse_tree(vertex, relation);
    }
    out[vertex] = timer++;
  }

private:

  T k;
  vector<vector<T>> up;
  vector<vector<W>> ws;

  T timer;
  vector<T> in;
  vector<T> out;

  Tree tree;
  DisjointSets sets;

  bool upper(T u, T v) const {
    return in[u] <= in[v] && out[u] >= out[v];
  }

  T lowest_ancestor(T u, T v) const {
    if (upper(u, v)) return u;
    if (upper(v, u)) return v;
    for (int i = k; i >= 0; --i)
      if (!upper(up[u][i], v))
        u = up.at(u).at(i);

    return up.at(u).at(0);
  }

  W find_max_edge(T u, T v) const {
    W max_weight = 0;
    if (u == v)
      return 0;
    for (int i = k; i >= 0; --i)
      if (upper(v, up.at(u).at(i))) {
        u = up.at(u).at(i);
        max_weight = max(max_weight, ws.at(u).at(k));
      }
    return max(max_weight, ws.at(u).at(0));
  }

  W max_weight(T from, T to) const {
    if (from == to)
      return 0;
    T ancestor = lowest_ancestor(from, to);
    return max(find_max_edge(from, ancestor), find_max_edge(to, ancestor));
  }

  bool is_valid_to_add(const Edge &e) {
    return sets.not_from_one_set(e.to, e.from);
  }

  void add_to_tree(const Edge &edge) {
    tree.add(edge);
    sets.union_sets(edge.to, edge.from);
  }

};

void solve(istream &is, ostream &os) {
  T vertices_qty = 0, edges_qty = 0;
  is >> vertices_qty >> edges_qty;
  Solver solver(vertices_qty);

  vector<Edge> edges(edges_qty);
  for (auto &edge: edges)
    is >> edge;

  solver.build_spanning_tree_on(edges);
  solver.traverse_tree();
  for (auto &edge: edges)
    cout << solver.weight_of_spanning_tree_with_fixed(edge) << endl;
}

int main() {
  solve(cin, cout);
  return 0;
}
