
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
#include <x86intrin.h>

//#define USE_IACA

#ifdef USE_IACA
#include <iacaMarks.h>
#endif

std::mt19937 rng;

void polynomial(float *ret, const float *const r_values, int num) {
  // r*r*r*(10+r*(-15+r*6));

  __m256 const_6 = _mm256_set1_ps(6.0f);
  __m256 const_neg_15 = _mm256_set1_ps(-15.0f);
  __m256 const_10 = _mm256_set1_ps(10.0f);
  // constants

  const int loop_factor = 8;

  for (int i = 0; i < num; i+=loop_factor) {

#ifdef USE_IACA
  IACA_START
#endif
    __m256 r;
    __m256 left;
    __m256 right;
    // aligned load of 256 bits r
    r = _mm256_load_ps(&r_values[i]);
    left = _mm256_mul_ps(r, r); // r * r
#ifndef __FMA__

    right = _mm256_mul_ps(r, const_6); // r * 6
    left = _mm256_mul_ps(left, r); // r * r * r
    right = _mm256_add_ps(right, const_neg_15); //-15 + r * 6
    right = _mm256_mul_ps(right, r); //r * (-15 + r * 6)
    right = _mm256_add_ps(right, const_10); //10 + (r * (-15 + r * 6))

#else
    right = _mm256_fmadd_ps(r, const_6, const_neg_15);
    left = _mm256_mul_ps(left, r);

    right = _mm256_fmadd_ps(r, right, const_10);

#endif
    right = _mm256_mul_ps(right, left); // r*r*r *(10 + r * (-15 + r * 6))

    _mm256_store_ps(&ret[i], right); // store 8 values to ret[i]

  }
#ifdef USE_IACA
  IACA_END
#endif
}

int main(int argc, char *argv[]) {
  const int test_size = 4096 * 2;
  std::normal_distribution<float> dist(0.5f, 0.5f);
  rng.seed(0);
  float *r_values = nullptr;
  float *ret = nullptr;
  float *ret2 = nullptr;
  double sum = 0.0f;
  std::function<float()> rnd = std::bind(dist, rng);

  r_values = static_cast<float*>(_mm_malloc(sizeof(float) * test_size, 32));

  ret = static_cast<float*>(_mm_malloc(sizeof(float) * test_size, 32));
  ret2 = static_cast<float*>(_mm_malloc(sizeof(float) * test_size, 32));

  if (!(r_values && ret && ret2))
    goto exit;

  std::generate_n(r_values, test_size, rnd);
  for (int i = 0; i < 100000; ++i) {
    polynomial(ret, r_values, test_size);
    // polynomial(ret2, ret, test_size);
    sum = std::accumulate(ret, ret + test_size, sum);
  }
  std::cout << sum << std::endl;


exit:
  _mm_free(r_values);
  _mm_free(ret);
  _mm_free(ret2);
}
