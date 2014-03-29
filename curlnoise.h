#ifndef CURLNOISE_H
#define CURLNOISE_H

// base classes for 2d and 3d examples

#include "vec.h"
#include "rigidshape2.h"
#include "simd/float8.hpp"
#include <iomanip>
struct CurlNoise2 {
  float t;       // time
  float delta_x; // used for finite difference approximations of curl
  float inv_2delta_x;
  std::vector<RigidShape2> rigid; // list of rigid shapes in flow

  CurlNoise2(void) : t(0), delta_x(1e-4), inv_2delta_x(1.0 / (2.0 * delta_x)) {}

  virtual ~CurlNoise2(void) {}

  virtual bool seed_particles(std::vector<Vec2f> &x, float dt) const {
    return false;
  }

  virtual float8 potential(float8 x, float8 y) const { return 0; }

  virtual void advance_time(float dt) { t += dt; }

  virtual void click(float x, float y) {}

  Vec4f get_velocity(const Vec4f &x) const {

    float8 sx(x[0], x[0], x[0] + delta_x, x[0] - delta_x, x[2], x[2], x[2] + delta_x, x[2] - delta_x);
    float8 sy(x[1] - delta_x, x[1] + delta_x, x[1], x[1], x[3] - delta_x, x[3] + delta_x, x[3], x[3]);

    float8 potential8 = potential(sx, sy) * inv_2delta_x;
    float8 hsub = _mm256_hsub_ps(potential8.m256,potential8.m256);

    return Vec4f(hsub[0],hsub[1],hsub[4],hsub[5]);
  }
};

#endif
