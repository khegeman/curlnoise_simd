
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
std::mt19937 rng;

float polynomial(float r) { return r * r * r * (10 + r * (-15 + r * 6)); }

int main(int argc, char *argv[]) {
  const int test_size = 4096 * 2;
  std::normal_distribution<float> dist(0.5f, 0.5f);
  rng.seed(0);
  std::vector<float> r_values(test_size);
  std::function<float()> rnd = std::bind(dist, rng);
  std::generate_n(r_values.begin(), test_size, rnd);

  float sum = 0.0f;

  for (int i = 0; i < 100000; ++i) {
    for (auto r : r_values) {
      // sum += polynomial(polynomial(r));
      sum += polynomial(r);
    }
  }
  std::cout << sum << std::endl;
}
