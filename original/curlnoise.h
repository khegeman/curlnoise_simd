#pragma once
// base classes for 2d and 3d examples

#include "vec.h"
#include "rigidshape2.h"

namespace original
{
struct CurlNoise2
{
   float t; // time
   float delta_x; // used for finite difference approximations of curl
   std::vector<RigidShape2> rigid; // list of rigid shapes in flow

   CurlNoise2(void)
      : t(0), delta_x(1e-4)
   {}

   virtual ~CurlNoise2(void)
   {}

   virtual bool seed_particles(std::vector<Vec2f> &x, float dt) const
   { return false; }

   virtual float potential(float x, float y) const
   { return 0; }

   virtual void advance_time(float dt)
   {
      t+=dt;
   }

   virtual void click(float x, float y)
   {}

   void get_velocity(const Vec2f &x, Vec2f &v) const
   {
      v[0]=-(potential(x[0], x[1]+delta_x) - potential(x[0], x[1]-delta_x))/(2*delta_x);
      v[1]= (potential(x[0]+delta_x, x[1]) - potential(x[0]-delta_x, x[1]))/(2*delta_x);
   }
};

}

