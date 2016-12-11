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

  typedef long long int T;
  typedef std::vector<T> row;
  typedef std::vector<row> matrix;

  Graph(long long int size): m(size) {

  }

  inline long long int size() const {
    return m.size();
  }

  inline void addEdge(long long int from, long long int to, long long int weight) {
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

  inline edge_it begin(long long int v) const {
    return m[v].begin();
  }

  inline edge_it end(long long int v) const {
    return m[v].end();
  }

  inline bool hasEdge(long long int from, long long int to) const {
    auto it = std::find(m[from].begin(), m[from].end(), to);
    return it != m[from].end();
  }

  matrix m;
};

class DepthSearch {

 public:

  static std::vector<long long int> search(const Graph& g, long long int to, long long int from) {
    DepthSearch instance(g, to);
    instance.visit(from);
    return std::move(instance.path);
  }

 private:

  std::vector<bool> visited;
  std::vector<long long int> path;

  Graph g;

  long long int to;

  DepthSearch(const Graph& g, long long int to): to(to),
  g(g),
  visited(g.size()),
  path(g.size()) {

  }

  void visit(long long int v) {
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

  DSU(long long int size): parents(size + 1) {
    for (long long int i = 0; i <= size; ++i)
      makeSet(i);
  }

  inline void makeSet(long long int v) {
    parents[v] = v;
  }

  long long int findSet(long long int x) {
    if (parents[x] == x)
      return x;
    return parents[x] = findSet(parents[x]);
  }

  inline void unionSets(long long int x, long long int y) {
    long long int a = findSet(x);
    long long int b = findSet(y);
    if (a != b) {
      parents[b] = a;
    }
  }

 private:

  typedef long long int T;
  std::vector<T> parents;

};

struct Edge {
  long long int from, to, weight;

  bool operator <(const Edge& e) const {
    return weight > e.weight;
  }

  inline bool operator ==(const Edge& e) const {
    return (from == e.from) && (to == e.to);
  }

  friend std::istream& operator >>(std::istream& is, Edge& e) {
    long long int from = 0, to = 0;
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
    long long int vertices = 0, edges = 0;
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
        long long int newWeight = s.weight;
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

  long long int size;
  long long int weight;

  Solver(long long int vertices, long long int edges): size(vertices), tree(vertices),
  sets(vertices), edges(edges),
  w(vertices, Graph::row(vertices)),
  weight(0) {

  }

  void buildTree() {
    long long int i = 0;
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

  long long int cycleMakerWeight(const Edge& newEdge) const {
    auto paths = DepthSearch::search(tree, newEdge.to, newEdge.from);
    long long int v = newEdge.to;
    long long int max = 0;
    while (v != newEdge.from) {
      max = std::max(max, w[v][paths[v]]);
      v = paths[v];
    }
    return max;
  }

  friend std::istream& operator >>(std::istream& is, Solver& s) {
    Edge e;
    for (long long int i = 0; i < s.edges.size(); ++i) {
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
