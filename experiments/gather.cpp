
#include <vector>
#include <iostream>

#include "simd/float8.hpp"

struct int8 {

    union { __m256i m256; int32_t v[8]; };
    int8(const __m256i a) : m256(a) {
        // empty
    }
};

int main(int argc, char *argv[]) {

	std::vector<float> values {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f};

  __m256i indices = _mm256_set_epi32(7,7,1,3,6,5,6,7);
  float8 gathered = _mm256_i32gather_ps(&values[0],indices, 4);

  for (float f : gathered.v)
    std::cout << f << std::endl;

  __m256i ivalues = _mm256_set_epi32(127,128,129,256,257,1,2,0);
  __m256i mask = _mm256_set1_epi32(127);

  std::cout << "mod 128 test" << std::endl;

  //modulo 128 test
  int8 mod = _mm256_and_si256(ivalues, mask);
  for (int  i : mod.v)
    std::cout << i << std::endl;

  return 0;
}
