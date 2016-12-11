#include <iostream>

int main() {
  size_t n, m, a;
  std::cin >> n >> m >> a;

  size_t rows_qty, cols_qty;
  rows_qty = n / a + (n % a != 0 ? 1 : 0);
  cols_qty = m / a + (m % a != 0 ? 1 : 0);
  std::cout << rows_qty * cols_qty;
  return 0;
}
