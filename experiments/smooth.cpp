
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
std::mt19937 rng;

float smooth_step(float r) {
  if (r < 0)
    return 0;
  else if (r > 1)
    return 1;
  return r * r * r * (10 + r * (-15 + r * 6));
}

int main(int argc, char *argv[]) {
  const int test_size = 4096 * 2;
	//	std::uniform_real_distribution<float> dist(-1.0f,2.0f);
	std::uniform_real_distribution<float> dist(-1.0f,0.0f);	
	rng.seed(0);
	std::vector<float> r_values(test_size);
	std::function<float()> rnd = std::bind(dist, rng);
	std::generate_n(r_values.begin(), test_size, rnd);
	//std::sort(r_values.begin(), r_values.end());
	double sum = 0.0f;
	for (int i = 0 ; i < 100000; ++i) {
		float loop_sum = 0.0f;
		for (auto r : r_values) {
			loop_sum += smooth_step(r);
		}
		sum += loop_sum;
  }
  std::cout << sum << std::endl;
}
