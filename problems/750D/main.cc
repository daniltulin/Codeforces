#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

class Solution {
 public:
  static int run(istream &is, ostream &os);

  Solution(int n): steps(n), qty_(0) {

  }

  void read_steps(istream &is) {
    for (auto &step: steps)
      is >> step;
  }

  void initialize_plane() {
    int sum = 0;
    for (auto &step: steps)
      sum += step;
    offset = sum + 1;
    visited.resize(2*offset + 1, vector<bool>(2*offset + 1));
  }

  void search() {
    dfs(offset, offset);
  }

  void dfs(int x, int y, int dx = 0, int dy = 1, int n = 0) {
    if (n >= steps.size()) return;
    int step_qty = steps[n];
    for (int i = 0; i < step_qty; ++i) {
      if (visited.at(x).at(y) == false) {
        qty_ += 1;
        visited[x][y] = true;
      }
      x += dx;
      y += dy;
    }
    if (dx == 0 && dy != 0) {
      dfs(x - 1, y, -1, dy, n + 1);
      dfs(x + 1, y, 1, dy, n + 1);
    } else if (dx != 0 && dy == 0) {
      dfs(x, y + 1, dx, 1, n + 1);
      dfs(x, y - 1, dx, -1, n + 1);
    } else {
      dfs(x - dx, y, 0, dy, n + 1);
      dfs(x, y - dy, dx, 0, n + 1);
    }
  }

  int qty() const {
    return qty_;
  }

 private:
  vector<int> steps;
  int offset;

  int qty_;

  vector<vector<bool>> visited;
};

int Solution::run(istream &is, ostream &os) {
  int n;
  is >> n;

  if (n == 1) {
    int step;
    is >> step;
    os << step;
    return 0;
  }

  Solution s(n);
  s.read_steps(is);
  s.initialize_plane();
  s.search();
  os << s.qty() << endl;

  return 0;
}
