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

  int p = 0;
  int t;
  char dir[20];
  char d;
  for (int i = 0; i < n; ++i) {
    is >> t >> dir;

    d = dir[0];
    if (d == 'S') {
      if (p == 20000) {os << "NO"; return 0;}
      p += t;
      if (p > 20000) {os << "NO"; return 0;}
    } else if (d == 'N') {
      if (p == 0) {os << "NO"; return 0;}
      p -= t;
      if (p < 0) {os << "NO"; return 0;}
    } else {
      if (p == 0 || p == 20000) {os << "NO"; return 0;}
    }
  }

  if (p == 0) os << "YES" << endl;
  else os << "NO" << endl;

  return 0;
}
