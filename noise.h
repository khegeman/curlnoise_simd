#ifndef NOISE_H
#define NOISE_H

#include "vec.h"
#include "simd/float8.hpp"
struct Noise2
{
   Noise2(unsigned int seed=171717);
   void reinitialize(unsigned int seed);
   float operator()(float x, float y) const;
   float8 operator()(float8 x, float8 y) const;   
   float operator()(const Vec2f &x) const { return (*this)(x[0], x[1]); }

   protected:
   static const unsigned int n=128;
   Vec2f basis[n];

   //copy boundary condition perm[n]=perm[0]
   uint8_t perm[n+1];

   unsigned int hash_index(int i, int j) const
   { return perm[(perm[i%n]+j)%n]; }
};

// FlowNoise classes - time varying versions of some of the above

struct FlowNoise2: public Noise2
{
   FlowNoise2(unsigned int seed=171717, float spin_variation=0.2);
   void set_time(float t); // period of repetition is approximately 1

   protected:
   Vec2f original_basis[n];
   float spin_rate[n];
};

#endif
