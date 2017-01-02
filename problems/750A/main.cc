#include <iostream>

using namespace std;

class Solution {
 public:
  static int run(istream &is, ostream &os);
};

int sum(int k) {
  return 5 * k * (k + 1) / 2;
}

int Solution::run(istream &is, ostream &os) {
  int n, k;
  is >> n >> k;
  int qty = 1;
  while((sum(qty) <= (240 - k)) && (qty <= n)) qty++;
  os << qty - 1 << endl;
  return 0;
}
