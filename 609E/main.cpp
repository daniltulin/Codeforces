#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef int T;
typedef long long int W;

struct Edge {
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
  DisjointSets(size_t size): parents(size) {
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
    if (a != b) {
      parents[b] = a;
      parents[y] = a;
    }
  }

  vector<T> parents;
};

struct Relation {
  Relation(): to(-1), w(-1) {

  }

  Relation(T to, W w): to(to), w(w) {

  }

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

class PathFinder {
public:
  PathFinder(const Tree &t, T from, T to): relations(t.size()), tree(&t),
                                           from(from), to(to) {

  }

  vector<Relation> find_path() {
    width_search();
    vector<Relation> path;
    T vertex = to;
    while(vertex != from) {
      path.push_back(relations[vertex]);
      vertex = path.back().to;
    }
    return path;
  }

private:

  void width_search() {
    q.push(from);
    while (q.size()) {
      T vertex = q.front();
      if (vertex == to)
        return;
      q.pop();
      traverse(vertex);
    }
  }

  void traverse(T vertex) {
    for (auto &relation: tree->relations_for(vertex)) {
      T neighbor = relation.to;
      if (is_not_visited(neighbor)) {
        relations[neighbor] = Relation(vertex, relation.w);
        q.push(neighbor);
        if (neighbor == to) return;
      }
    }
  }

  bool is_not_visited(T vertex) const {
    return relations[vertex].to < 0;
  }

  T from, to;

  const Tree *tree;
  queue<int> q;
  vector<Relation> relations;
};

class Solver {
public:
  Solver(T vertices_qty): tree(vertices_qty), sets(vertices_qty) {

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

    auto path = find_path(edge.from, edge.to);
    W max_weight = 0;
    for (auto &relation: path)
      max_weight = max(max_weight, relation.w);
    return tree.weight + edge.w - max_weight;
  }

private:

  vector<Edge> edges;
  Tree tree;
  DisjointSets sets;

  vector<Relation> find_path(T from, T to) {
    return PathFinder(tree, from, to).find_path();
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
  for (auto &edge: edges)
    cout << solver.weight_of_spanning_tree_with_fixed(edge) << endl;
}

int main() {
  solve(cin, cout);
  return 0;
}
