#include <iostream>

using namespace std;

class Solution {
 public:
  static int run(istream &is, ostream &os);
};

int Solution::run(istream &is, ostream &os) {
  int w;
  is >> w;
  if (w % 2 == 0 && w != 2)
    os << "YES" << endl;
  else
    os << "NO" << endl;
  return 0;
}
