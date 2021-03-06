#include <iostream>

using namespace std;

void solve(istream &is, ostream &os) {
  long long int n = 0, m = 0, a = 1;
  is >> n >> m >> a;

  long long int rows_qty = 0, cols_qty = 0;
  rows_qty = n / a + (n % a != 0 ? 1 : 0);
  cols_qty = m / a + (m % a != 0 ? 1 : 0);
  os << rows_qty * cols_qty;
}

class Solution {
 public:
   static int run(istream &is, ostream &os);
};

int Solution::run(istream &is, ostream &os) {
  solve(is, os);
  return 0;
}
