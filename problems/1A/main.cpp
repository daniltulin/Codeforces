#include <iostream>

int main() {
  long long int n = 0, m = 0, a = 1;
  std::cin >> n >> m >> a;

  long long int rows_qty = 0, cols_qty = 0;
  rows_qty = n / a + (n % a != 0 ? 1 : 0);
  cols_qty = m / a + (m % a != 0 ? 1 : 0);
  std::cout << rows_qty * cols_qty;
  return 0;
}
