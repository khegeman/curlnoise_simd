#include "noise.h"
#include "interp.h"

namespace original
{
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
}

float Noise2::
operator()(float x, float y) const
{
   float floorx=std::floor(x), floory=std::floor(y);
   int i=(int)floorx, j=(int)floory;
   const Vec2f &n00=basis[hash_index(i,j)];
   const Vec2f &n10=basis[hash_index(i+1,j)];
   const Vec2f &n01=basis[hash_index(i,j+1)];
   const Vec2f &n11=basis[hash_index(i+1,j+1)];
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
}