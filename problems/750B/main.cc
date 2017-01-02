#include <iostream>
#include <unistd.h>

using namespace std;

class Solution {
 public:
  static int run(istream &is, ostream &os);
};

void failure(ostream &os) {
  os << "NO" << endl;
  exit(0);
}

int Solution::run(istream &is, ostream &os) {
  int n = 0;
  is >> n;

  int p = 0;
  int t;
  char dir[20];
  char d;
  for (int i = 0; i < n; ++i) {
    is >> t >> dir;
    d = dir[0];
    if (d == 'S') {
      if (p == 20000) failure(os);
      p += t;
    } else if (d == 'N') {
      if (p == 0) failure(os);
      p -= t;
    } else {
      if (p == 0 || p == 20000) failure(os);
    }
  }

  if (p == 0) os << "YES" << endl;

  return 0;
}
