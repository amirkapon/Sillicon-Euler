#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

const size_t BOUND = 28124;
const double PI = acos(-1);

using std::vector;
using cd = std::complex<double>;
using std::swap;

/**
 * Perform FFT on a, and invert it if "invert" is set to true.
 */
void fft(vector<cd> &a, bool invert) {
  int n = a.size();

  for (int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1)
      j ^= bit;
    j ^= bit;

    if (i < j)
      swap(a[i], a[j]);
  }

  for (int len = 2; len <= n; len <<= 1) {
    double ang = 2 * PI / len * (invert ? -1 : 1);
    cd wlen(cos(ang), sin(ang));
    for (int i = 0; i < n; i += len) {
      cd w(1);
      for (int j = 0; j < len / 2; j++) {
        cd u = a[i + j], v = a[i + j + len / 2] * w;
        a[i + j] = u + v;
        a[i + j + len / 2] = u - v;
        w *= wlen;
      }
    }
  }

  if (invert) {
    for (cd &x : a)
      x /= n;
  }
}

/**
 * Return the square of the polynomial whose coefficients are a.
 */
vector<size_t> square(vector<size_t> const &a) {
  vector<cd> fa(a.begin(), a.end());
  int n = 1;
  while (n < a.size() * 2)
    n <<= 1;

  fa.resize(n);
  fft(fa, false);
  for (int i = 0; i < n; i++)
    fa[i] *= fa[i];
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

/**
 * Return a boolean array such that:
 * array[i] = true iff i is an abundant number.
 */
std::array<bool, BOUND>
calculate_is_abundant(const std::array<size_t, BOUND> &sum_of_divisors) {
  std::array<bool, BOUND> is_abundant;
  for (size_t i = 1; i < BOUND; i++) {
    is_abundant[i] = sum_of_divisors[i] > i;
  }
  return is_abundant;
}

/**
 * Calculate the sum of all values that cannot be expressed as the sum of two
 * abundant numbers.
 */
int64_t find_sum() {
  std::array<size_t, BOUND> sum_of_divisors = sieve_of_eratosthenes();
  std::array<bool, BOUND> is_abundant = calculate_is_abundant(sum_of_divisors);
  std::array<bool, BOUND> is_reachable = {0};
  std::vector<size_t> abundants(is_abundant.begin(), is_abundant.end());
  std::vector<size_t> sum_of_pairs = square(abundants);
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