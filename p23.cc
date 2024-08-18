#include <array>
#include <iostream>
#include <vector>
#include <chrono>

const size_t BOUND = 28124;

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

// std::vector<size_t> get_abundant(const std::array<bool, BOUND>& is_abundant)
// { std::vector<
// }

int64_t find_sum(const std::array<bool, BOUND> &is_abundant) {
  std::array<bool, BOUND> is_reachable = {0};
  for (size_t i = 0; i < BOUND; i++) {
    for (size_t j = 0; j <= i && i + j < BOUND; j++) {
      if (is_abundant[i] && is_abundant[j]) {
        is_reachable[i + j] = true;
      }
    }
  }
  int64_t sum = 0;
  for (size_t i = 0; i < BOUND; i++) {
    if (!is_reachable[i]) {
      sum += i;
    }
  }
  return sum;
}
int main() {
  
  std::array<size_t, BOUND> sum_of_divisors = sieve_of_eratosthenes();
  std::array<bool, BOUND> is_abundant = calculate_is_abundant(sum_of_divisors);
  int64_t sum = find_sum(is_abundant);
  std::cout << "Sum is: " << sum << std::endl;
  return 0;
}