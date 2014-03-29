
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
std::mt19937 rng;

//#define USE_IACA

#ifdef USE_IACA
#include <iacaMarks.h>
#endif

static float polynomial(float r) {
  return r * r * r * (10 + r * (-15 + r * 6));
}

static float smooth_step(float r) {
  float rv = polynomial(r);
  return std::max(std::min(rv, 1.0f), 0.0f);
}


int main(int argc, char *argv[]) {
  const int test_size = 4096 * 2;

  std::uniform_real_distribution<float> dist(-1.0f, 2.0f);
  //	std::uniform_real_distribution<float> dist(-1.0f,0.0f);
  rng.seed(0);
  std::vector<float> r_values(test_size);
  std::vector<float> x_values(test_size);
  std::function<float()> rnd = std::bind(dist, rng);
  std::generate_n(r_values.begin(), test_size, rnd);

  double sum = 0.0;
  for (int i = 0; i < 100000; ++i) {
  	float loop_sum = 0.0f;
    for (auto r : r_values) {

#ifdef USE_IACA
      IACA_START
#endif
      loop_sum += smooth_step(r);
    }

#ifdef USE_IACA
    IACA_END
#endif
    sum += loop_sum;
  }
  std::cout << sum << std::endl;
}
