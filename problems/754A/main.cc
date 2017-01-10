#include <iostream>

using namespace std;

class Solution {
 public:
  static int run(istream &is, ostream &os);
};

int sum(int k) {
  return k * (k + 1) / 2;
}

int Solution::run(istream &is, ostream &os) {
  int n;
  is >> n;

  int qty = 0;
  while(sum(qty + 1) <= n) qty++;

  int s = sum(qty + 1) - n;
  os << qty + (s > 0 ? 0 : 1) << endl;
  for(int i = 1; i <= qty + 1; ++i)
    if (i != s)
      os << i << " ";

  return 0;
}
