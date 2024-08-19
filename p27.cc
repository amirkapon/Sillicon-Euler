#include <array>
#include <chrono>
#include <iostream>
#include <vector>

const size_t BOUND = 1e3;
std::array<bool, BOUND + 1> is_prime = {true};

std::vector<int> get_primes() {
  std::vector<int> primes;
  for (size_t i = 0; i <= BOUND; i++) {
    is_prime[i] = true;
  }
  is_prime[0] = is_prime[1] = false;
  for (size_t i = 2; i <= BOUND; i++) {
    if (!is_prime[i]) {
      continue;
    }
    // std::cout << "i: " << i << std::endl;
    primes.push_back(i);
    primes.push_back(-i);
    for (size_t j = 2 * i; j <= BOUND; j += i) {
      is_prime[j] = false;
    }
  }
  return primes;
}

bool check_if_prime(size_t value) {
  //   if (value <= BOUND) {
  //     return is_prime[value];
  //   }
  for (size_t i = 2; i * i <= value; i++) {
    if (value % i == 0)
      return false;
  }
  return true;
}
std::vector<int> generate_a_values() {
  std::vector<int> a(2 * BOUND - 1);
  for (int i = 0; i < 2 * BOUND - 1; i++) {
    a[i] = i - BOUND - 1;
  }
  return a;
}

std::vector<int> generate_b_values() { return get_primes(); }

size_t calculate_sequence_length(int a, int b) {
  for (int n = 0; true; n++) {
    int number = n * n + a * n + b;
    number = abs(number);
    if (!check_if_prime(number)) {
      return n;
    }
  }
  return -1;
}

int calculate_max_sequence_length() {
  std::vector<int> a = generate_a_values();
  std::vector<int> b = generate_b_values();
  size_t max_sequence_length = 0;
  int optimal_a, optimal_b;
  for (auto val_a : a) {
    for (auto val_b : b) {
      size_t current_length = calculate_sequence_length(val_a, val_b);
      if (current_length > max_sequence_length) {
        optimal_a = val_a;
        optimal_b = val_b;
        max_sequence_length = current_length;
      }
    }
  }
  std::cout << "optimal a: " << optimal_a << std::endl;
  std::cout << "optimal b: " << optimal_b << std::endl;
  return optimal_a * optimal_b;
}

int main() {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  int max_sequence_length = calculate_max_sequence_length();
  end = std::chrono::system_clock::now();

  std::cout << "Maximum sequnece length: " << max_sequence_length << std::endl;

  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Elapsed time: " << elapsed_seconds.count() << std::endl;
  return 0;
}
