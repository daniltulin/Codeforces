#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

template<typename T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
  if (v.empty())
    return os;
  os << "[";
  for (typename std::vector<T>::const_iterator it = v.begin();
      it != v.end(); ++it) {
    os << *it << ((it - v.begin() < v.size() - 1) ? ", " : "] ");
  }
  os << v.size();
  return os;
}

class Graph {

 public:

  typedef int T;
  typedef std::vector<T> row;
  typedef std::vector<row> matrix;

  Graph(int size): m(size) {

  }

  inline int size() const {
    return m.size();
  }

  inline void addEdge(int from, int to, int weight) {
    m[from].push_back(to);
    m[to].push_back(from);
  }

  typedef row::const_iterator edge_it;
  typedef matrix::const_iterator it;

  inline it begin() const {
    return m.begin();
  }

  inline it end() const {
    return m.end();
  }

  inline edge_it begin(int v) const {
    return m[v].begin();
  }

  inline edge_it end(int v) const {
    return m[v].end();
  }

  inline bool hasEdge(int from, int to) const {
    auto it = std::find(m[from].begin(), m[from].end(), to);
    return it != m[from].end();
  }

  matrix m;
};

class DepthSearch {

 public:

  static std::vector<int> search(const Graph& g, int to, int from) {
    DepthSearch instance(g, to);
    instance.visit(from);
    return std::move(instance.path);
  }

 private:

  std::vector<bool> visited;
  std::vector<int> path;

  Graph g;

  int to;

  DepthSearch(const Graph& g, int to): to(to),
  g(g),
  visited(g.size()),
  path(g.size()) {

  }

  void visit(int v) {
    visited[v] = true;
    if (v == to)
      return;
    for (Graph::edge_it it = g.begin(v); it != g.end(v); ++it) {
      if (!visited[*it]) {
        path[*it] = v;
        visit(*it);
      }
    }
  }

};

class DSU {

 public:

  DSU(int size): parents(size + 1) {
    for (int i = 0; i <= size; ++i)
      makeSet(i);
  }

  inline void makeSet(int v) {
    parents[v] = v;
  }

  int findSet(int x) {
    if (parents[x] == x)
      return x;
    return parents[x] = findSet(parents[x]);
  }

  inline void unionSets(int x, int y) {
    int a = findSet(x);
    int b = findSet(y);
    if (a != b) {
      parents[b] = a;
    }
  }

 private:

  typedef int T;
  std::vector<T> parents;

};

struct Edge {
  int from, to, weight;

  bool operator <(const Edge& e) const {
    return weight > e.weight;
  }

  inline bool operator ==(const Edge& e) const {
    return (from == e.from) && (to == e.to);
  }

  friend std::istream& operator >>(std::istream& is, Edge& e) {
    int from = 0, to = 0;
    is >> from >> to >> e.weight;
    e.to = to - 1;
    e.from = from - 1;
    return is;
  }

  friend std::ostream& operator <<(std::ostream& os,
      const Edge& e) {
    os << e.from << " " << e.to;
    return os;
  }
};


class Solver {

 public:
  static void solve(std::istream& is, std::ostream& os) {
    int vertices = 0, edges = 0;
    is >> vertices >> edges;
    Solver s(vertices, edges);
    is >> s;
    s.buildTree();
    for (std::vector<Edge>::const_iterator it = s.edges.begin();
        it != s.edges.end();
        ++it) {
      if (s.tree.hasEdge(it->from, it->to)) {
        os << s.weight << std::endl;
      } else {
        int newWeight = s.weight;
        newWeight += it->weight - s.cycleMakerWeight(*it);
        os << newWeight << std::endl;
      }
    }
  }

 private:
  Graph tree;
  DSU sets;
  Graph::matrix w;

  std::priority_queue<Edge> q;
  std::vector<Edge> edges;

  int size;
  int weight;

  Solver(int vertices, int edges): size(vertices),
                                   tree(vertices),
                                   sets(vertices),
                                   edges(edges),
                w(vertices, Graph::row(vertices)),
                weight(0) {

  }

  void buildTree() {
    int i = 0;
    while (i < size - 1) {
      Edge e = q.top();
      if (sets.findSet(e.to) != sets.findSet(e.from)) {
        i++;
        tree.addEdge(e.to, e.from, e.weight);
        weight += e.weight;
        sets.unionSets(e.to, e.from);
      }
      q.pop();
    }
  }

  int cycleMakerWeight(const Edge& newEdge) const {
    auto paths = DepthSearch::search(tree, newEdge.to, newEdge.from);
    int v = newEdge.to;
    int max = 0;
    while (v != newEdge.from) {
      max = std::max(max, w[v][paths[v]]);
      v = paths[v];
    }
    return max;
  }

  friend std::istream& operator >>(std::istream& is, Solver& s) {
    Edge e;
    for (int i = 0; i < s.edges.size(); ++i) {
      is >> e;
      s.q.push(e);
      s.edges[i] = e;
      s.w[e.from][e.to] = e.weight;
      s.w[e.to][e.from] = e.weight;
    }
    return is;
  }

};

int main() {
  Solver::solve(std::cin, std::cout);
  return 0;
}
