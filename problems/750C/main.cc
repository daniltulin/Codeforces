#include <iostream>
#include <unistd.h>

using namespace std;

class Solution {
 public:
  static int run(istream &is, ostream &os);
};

int Solution::run(istream &is, ostream &os) {
  int n = 0;
  is >> n;

  int level = 0;
  int c, d;

  int infinum = INT_MAX, supremum = INT_MIN;

  for (int i = 0; i < n; ++i) {
    is >> c >> d;
    if (d == 1) infinum = min(infinum, level);
    else supremum = max(supremum, level);
    level += c;
  }

  if (infinum <= supremum) {os << "Impossible" << endl; return 0;}
  if (supremum == INT_MIN) {os << "Infinity" << endl; return 0;}

  os << level + 1899 - supremum << endl;

  return 0;
}
