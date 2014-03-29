
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
#include <x86intrin.h>

#define USE_IACA

#ifdef USE_IACA
#include <iacaMarks.h>
#endif

std::mt19937 rng;
void polynomial(float *ret, const float *const x_values,const float* y_values, int num) {

  __m256 const_2 = _mm256_set1_ps(2.0f);

  for (int i = 0; i < num; i++) {
#ifdef USE_IACA
  IACA_START
#endif
/*
      float x = x_values[i];
      float y = y_values[i];        
  		ret[i] = x * x * x + 2 * y; 
*/
      __m256 x = _mm256_load_ps(&x_values[i]);
      __m256 y = _mm256_load_ps(&y_values[i]);      
      __m256 left = _mm256_mul_ps(x, x); 
      left = _mm256_mul_ps(left, x); 
      //__m256 right = _mm256_mul_ps(const_2, y);
      __m256 right = _mm256_add_ps(y, y);
      right = _mm256_add_ps(right, left);
      _mm256_store_ps(&ret[i], right);     
   } 
 #ifdef USE_IACA
  IACA_END
#endif   
}


int main(int argc, char *argv[]) {
  const int test_size = 4096 * 2;
  std::normal_distribution<float> dist(0.5f, 0.5f);
  rng.seed(0);
  float *x_values = nullptr;
  float *ret = nullptr;
  float *y_values = nullptr;
  double sum = 0.0f;
  std::function<float()> rnd = std::bind(dist, rng);


  x_values = static_cast<float*>(_mm_malloc(sizeof(float) * test_size, 32));
  y_values = static_cast<float*>(_mm_malloc(sizeof(float) * test_size, 32));

  ret = static_cast<float*>(_mm_malloc(sizeof(float) * test_size, 32));
  
  if (!(x_values && ret && y_values))
    goto exit;

  std::generate_n(x_values, test_size, rnd);
  std::generate_n(y_values, test_size, rnd);  
  for (int i = 0; i < 100000; ++i) {
    polynomial(ret, x_values,y_values, test_size);
    // polynomial(ret2, ret, test_size);
    sum = std::accumulate(ret, ret + test_size, sum);
  }
  std::cout << sum << std::endl;


exit:
  _mm_free(x_values);
  _mm_free(ret);
  _mm_free(y_values);

}
