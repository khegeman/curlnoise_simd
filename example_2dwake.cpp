#include "example_2dwake.h"
#include "bluenoise.h"
#include "simd/float8.hpp"

Example2DWake::
Example2DWake(void)
   : disc_centre(0.7,0.5),
     disc_radius(0.12),
     inv_disc_radius(1.0f/disc_radius),
     disc_influence(0.25),
     inv_disc_influence(1.0f / disc_influence),
     ymin(0),
     ymax(0.75),
     initial_xmin(1.5),
     initial_xmax(3.5),
     background_flow_speed(-0.25),
     wake_expansion(0.3)
{
   rigid.resize(1);
   rigid[0].centre=disc_centre;
   rigid[0].radius=disc_radius;
   noise_lengthscale.push_back(1.0 /0.1);
   noise_gain.push_back(0.03);
   noise_lengthscale.push_back(1.0 / 0.06);
   noise_gain.push_back(0.03*0.35);
   noise_lengthscale.push_back(1.0 / 0.026);
   noise_gain.push_back(0.03*0.1);
}

static float8 dist(float8 x, float8 y, float8 disc_x, float8 disc_y) {
  float8 dist2 = sqr(x - disc_x) + sqr(y - disc_y);
  return sqrt(dist2);
}

float8 Example2DWake::
solid_distance8(float8 x, float8 y) const
{
   return dist(x, y,float8(disc_centre.v[0]),float8(disc_centre.v[1])) - float8(disc_radius);
}

float Example2DWake::
solid_distance(float x, float y) const
{
   return dist(Vec2f(x,y),disc_centre)-disc_radius;
}

bool Example2DWake::
seed_particles(std::vector<Vec2f> &x, float dt) const
{
   if(t==0){
      bluenoise_sample(0.01f, Vec2f(initial_xmin, ymin), Vec2f(initial_xmax, ymax), x);
      for(int i=0; i<(int)x.size(); ++i){
         if(solid_distance(x[i][0], x[i][1])<=0){
            erase_unordered(x, i);
            --i;
         }
      }
      return true;
   }else
      return false; // no particles added after initial seeding at t=0
}

float8 Example2DWake::potential(float8 x, float8 y) const {
  // begin with background laminary flow (adjusted so zero level goes through
  // centre of disc)
  float8 p = -background_flow_speed * (y - disc_centre[1]);
  // modify for disc

  float8 d = solid_distance8(x, y);

  p *= ramp(d * inv_disc_influence);

  // add turbulent wake that respects disc
  float8 wake_x = smooth_step(1.0f - (x - disc_centre[0]) * inv_disc_radius);

  float8 wake_y = smooth_step(
      1.0f - abs(y - disc_centre[1]) /
              (1.5f * disc_radius + wake_expansion * (disc_centre[0] - x)));

  float8 wake = wake_x * wake_y;

  float8 s = 0;
  for (unsigned int i = 0; i < noise_lengthscale.size(); ++i) {
    s += ramp(d*noise_lengthscale[i])*noise_gain[i]*noise((x-background_flow_speed*t)*noise_lengthscale[i], y*noise_lengthscale[i]);
  }
  p += wake * s;

  return p;
}

void Example2DWake::
advance_time(float dt)
{
   t+=dt;
  noise.set_time(0.5f*noise_gain[0] * noise_lengthscale[0]*t);
}

