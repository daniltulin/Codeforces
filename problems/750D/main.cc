#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

struct D {
  D(int dx, int dy): dx(dx), dy(dy) { }
  int dx;
  int dy;
};

D directions[] = {D(0, 1), D(1, 1), D(1, 0), D(1, -1),
                  D(0, -1), D(-1 , -1), D(-1, 0), D(-1, 1)};

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

    typedef vector<bool> Row;
    typedef vector<Row> Matrix;
    typedef vector<Matrix> Matrix3D;
    visited_directions.resize(2*offset + 1,
                              Matrix3D(2*offset + 1,
                                       Matrix(steps.size(), Row(8))));
  }

  void search() {
    dfs(offset, offset);
  }

  void dfs(int x, int y, int d = 0, int n = 0) {
    if (n >= steps.size() || visited_directions[x][y][n][d]) return;
    visited_directions[x][y][n][d] = true;
    int step_qty = steps[n];

    int dx = directions[d].dx, dy = directions[d].dy;

    x += dx; y += dy;
    for (int i = 0; i < step_qty; ++i) {
      if (visited[x][y] == false) {
        qty_ += 1;
        visited[x][y] = true;
      }
      if (i != step_qty - 1) {
        x += dx; y += dy;
      }
    }
    dfs(x, y, (d + 7) % 8, n + 1);
    dfs(x, y, (d + 1) % 8, n + 1);
  }

  int qty() const {
    return qty_;
  }

 private:
  vector<int> steps;
  int offset;

  int qty_;

  vector<vector<bool>> visited;
  vector<vector<vector<vector<bool>>>> visited_directions;
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
