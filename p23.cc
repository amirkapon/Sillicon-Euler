#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

const size_t BOUND = 28124;
using std::vector;
using cd = std::complex<double>;
const double PI = acos(-1);

void fft(std::vector<cd> &a, bool invert) {
  int n = a.size();
  if (n == 1)
    return;

  std::vector<cd> a0(n / 2), a1(n / 2);
  for (int i = 0; 2 * i < n; i++) {
    a0[i] = a[2 * i];
    a1[i] = a[2 * i + 1];
  }
  fft(a0, invert);
  fft(a1, invert);

  double ang = 2 * PI / n * (invert ? -1 : 1);
  cd w(1), wn(cos(ang), sin(ang));
  for (int i = 0; 2 * i < n; i++) {
    a[i] = a0[i] + w * a1[i];
    a[i + n / 2] = a0[i] - w * a1[i];
    if (invert) {
      a[i] /= 2;
      a[i + n / 2] /= 2;
    }
    w *= wn;
  }
}

vector<size_t> multiply(vector<size_t> const &a, vector<size_t> const &b) {
  vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
  int n = 1;
  while (n < a.size() + b.size())
    n <<= 1;
  fa.resize(n);
  fb.resize(n);

  fft(fa, false);
  fft(fb, false);
  for (int i = 0; i < n; i++)
    fa[i] *= fb[i];
  fft(fa, true);

  vector<size_t> result(n);
  for (int i = 0; i < n; i++)
    result[i] = round(fa[i].real());
  return result;
}

/**
 * Return an array that satisfies:
 * array[i] = sum of divisors of i
 */
std::array<size_t, BOUND> sieve_of_eratosthenes() {
  std::array<size_t, BOUND> sum_of_divisors = {0};
  for (size_t i = 1; i < BOUND; i++) {
    for (size_t j = 2 * i; j < BOUND; j += i) {
      sum_of_divisors[j] += i;
    }
  }
  return sum_of_divisors;
}

std::array<bool, BOUND>
calculate_is_abundant(const std::array<size_t, BOUND> &sum_of_divisors) {
  std::array<bool, BOUND> is_abundant;
  for (size_t i = 1; i < BOUND; i++) {
    is_abundant[i] = sum_of_divisors[i] > i;
  }
  return is_abundant;
}

int64_t find_sum() {
  std::array<size_t, BOUND> sum_of_divisors = sieve_of_eratosthenes();
  std::array<bool, BOUND> is_abundant = calculate_is_abundant(sum_of_divisors);
  std::array<bool, BOUND> is_reachable = {0};
  std::vector<size_t> abundants(is_abundant.begin(), is_abundant.end());
  std::vector<size_t> sum_of_pairs = multiply(abundants, abundants);
  int64_t sum = 0;
  for (size_t i = 0; i < BOUND; i++) {
    if (sum_of_pairs[i] == 0) {
      sum += i;
    }
  }
  return sum;
}

int main() {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  int64_t sum = find_sum();
  end = std::chrono::system_clock::now();

  std::cout << "Sum is: " << sum << std::endl;

  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Elapsed time: " << elapsed_seconds.count() << std::endl;
  return 0;
}