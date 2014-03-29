#ifndef EXAMPLE_2DWAKE_H
#define EXAMPLE_2DWAKE_H

#include "curlnoise.h"
#include "noise.h"

struct Example2DWake: public CurlNoise2
{
   Vec2f disc_centre;
   float disc_radius;
   float disc_influence;   
   float inv_disc_radius;   
   float inv_disc_influence;
   float ymin, ymax;
   float initial_xmin, initial_xmax;
   float background_flow_speed;
   float wake_expansion;
   std::vector<float> noise_lengthscale;
   std::vector<float> noise_gain;
   FlowNoise2 noise;

   Example2DWake(void);
   float8 solid_distance8(float8 x, float8 y) const;
   float solid_distance(float x, float y) const;   
   float solid_distance_sqr(float x, float y) const;
   // virtual functions overriding base class
   bool seed_particles(std::vector<Vec2f> &x, float dt) const;
   float8 potential(float8 x, float8 y) const;
   void advance_time(float dt);
};

#endif
