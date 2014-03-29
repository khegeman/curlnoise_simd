#include "noise.h"
#include "interp.h"

template<unsigned int N>
static Vec<N,float> sample_sphere(unsigned int &seed)
{
   Vec<N,float> v;
   float m2;
   do{
      for(unsigned int i=0; i<N; ++i){
         v[i]=randhashf(seed++,-1,1);
      }
      m2=mag2(v);
   }while(m2>1 || m2==0);
   return v/std::sqrt(m2);
}

//============================================================================

Noise2::
Noise2(unsigned int seed)
{
   for(unsigned int i=0; i<n; ++i){
      float theta=(float)(i*2*M_PI)/n;
      basis[i][0]=std::cos(theta);
      basis[i][1]=std::sin(theta);
      perm[i]=i;
   }
   reinitialize(seed);
}

void Noise2::
reinitialize(unsigned int seed)
{
   for(unsigned int i=1; i<n; ++i){
      int j=randhash(seed++)%(i+1);
      swap(perm[i], perm[j]);
   }
   //boundary condition
   perm[n]=perm[0];
}

float8 Noise2::
operator()(float8 x, float8 y) const
{
   float8 floorx=floor(x), floory=floor(y);

   float8 n00_0(0.f),n00_1(0.f),n01_0(0.f),n01_1(0.f),n10_0(0.f),n10_1(0.f),n11_0(0.f),n11_1(0.f);

   for (int iter = 0; iter < 8; ++iter) {

  int i = (int)floorx.v[iter], j = (int)floory.v[iter];
  //__m256i i = _mm256_cvtps_epi32(floorx.m256);
  //__m256i j = _mm256_cvtps_epi32(floory.m256);
  
  uint32_t i_index = i % n;
  //__m256i mask = _mm256_set1_epi32(127);
  //__m256i i_index = _mm256_and_si256(i, mask);  


  //__m256i i_indices = _mm256_i32gather_ps(&perm[0], i_index, 2);
  // can now do 1 16 bits lookup
  uint16_t i_indices = *(reinterpret_cast<const uint16_t *>(&perm[i_index]));

  int i_1 = i_indices & 0xFF;
  int i_2 = i_indices >> 8;

  uint32_t mod_j = (i_1 + j) % n;
  uint32_t mod_j1 = (i_2 + j) % n;

  uint16_t j_indices = *(reinterpret_cast<const uint16_t *>(&perm[mod_j]));

  int j_1 = j_indices & 0xFF;
  int j_2 = j_indices >> 8;

  n00_0.v[iter] = basis[j_1][0];
  n00_1.v[iter] = basis[j_1][1];
  
  n01_0.v[iter] = basis[j_2][0];
  n01_1.v[iter] = basis[j_2][1];    

  uint16_t j1_indices = *(reinterpret_cast<const uint16_t *>(&perm[mod_j1]));

  int j1_1 = j1_indices & 0xFF;
  int j1_2 = j1_indices >> 8;

  n10_0.v[iter] = basis[j1_1][0];
  n10_1.v[iter] = basis[j1_1][1];
  
  n11_0.v[iter] = basis[j1_2][0];
  n11_1.v[iter] = basis[j1_2][1];    

   }

  // std::cout << "original " << hash_index(i+1,j+1) << " modified " << (int)perm[mod_j1+1] << std::endl;
   float8 fx=x-floorx, fy=y-floory;
   float8 sx=fx*fx*fx*(10-fx*(15-fx*6)),
         sy=fy*fy*fy*(10-fy*(15-fy*6));
   return bilerp(    fx*n00_0 +     fy*n00_1,
                 (fx-1)*n10_0 +     fy*n10_1,
                     fx*n01_0 + (fy-1)*n01_1,
                 (fx-1)*n11_0 + (fy-1)*n11_1,
                 sx, sy);
}

float Noise2::
operator()(float x, float y) const
{
   float floorx=std::floor(x), floory=std::floor(y);
   int i=(int)floorx, j=(int)floory;

   uint32_t i_index = i % n;


   

   //can now do 1 16 bits lookup
   uint16_t i_indices = *(reinterpret_cast<const uint16_t*>(&perm[i_index]));

   int i_1 = i_indices & 0xFF;
   int i_2 = i_indices >> 8;

   uint32_t mod_j = (i_1 + j) % n;
   uint32_t mod_j1 = (i_2+j) % n;
   
   uint16_t j_indices = *(reinterpret_cast<const uint16_t*>(&perm[mod_j]));

   int j_1 = j_indices & 0xFF;
   int j_2 = j_indices >> 8;

   const Vec2f &n00=basis[j_1];
   const Vec2f &n01=basis[j_2];

   uint16_t j1_indices = *(reinterpret_cast<const uint16_t*>(&perm[mod_j1]));

   int j1_1 = j1_indices & 0xFF;
   int j1_2 = j1_indices >> 8;

   const Vec2f &n10=basis[j1_1];
   const Vec2f &n11=basis[j1_2 ];


  // std::cout << "original " << hash_index(i+1,j+1) << " modified " << (int)perm[mod_j1+1] << std::endl;
   float fx=x-floorx, fy=y-floory;
   float sx=fx*fx*fx*(10-fx*(15-fx*6)),
         sy=fy*fy*fy*(10-fy*(15-fy*6));
   return bilerp(    fx*n00[0] +     fy*n00[1],
                 (fx-1)*n10[0] +     fy*n10[1],
                     fx*n01[0] + (fy-1)*n01[1],
                 (fx-1)*n11[0] + (fy-1)*n11[1],
                 sx, sy);
}

//============================================================================

FlowNoise2::
FlowNoise2(unsigned int seed, float spin_variation)
   : Noise2(seed)
{
   seed+=n;
   for(unsigned int i=0; i<n; ++i){
      original_basis[i]=basis[i];
      spin_rate[i]=2.0f*(float)M_PI*randhashf(seed++, 1-0.5f*spin_variation, 1+0.5f*spin_variation);
   }
}

void FlowNoise2::
set_time(float t)
{
   for(unsigned int i=0; i<n; ++i){
      float theta=spin_rate[i]*t;
      float c=std::cos(theta), s=std::sin(theta);
      basis[i][0]= c*original_basis[i][0]+s*original_basis[i][1];
      basis[i][1]=-s*original_basis[i][0]+c*original_basis[i][1];
   }
}
