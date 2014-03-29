


#include <iostream>
#include <fstream>

#include "example_2dwake.h"
using namespace original;


float simulation_timestep=0.04;


std::vector<Vec2f> points;

CurlNoise2 *example=0;

void advance_simulation()
{
   static int pass = 0;
   pass++;
   Vec2f v, midx;   
   for(unsigned int i=0; i<points.size(); ++i){
      // almost RK2 (not updating t halfway through time step, so it's technically only 1st order accurate)
      example->get_velocity(points[i], v);
      Vec2f p1 = points[i];
      Vec2f v1 = v;
      midx=points[i]+0.5f*simulation_timestep*v;
      example->get_velocity(midx, v);
      points[i]+=simulation_timestep*v;
   }
   example->seed_particles(points, simulation_timestep);
   example->advance_time(simulation_timestep);
}

int main(int argc, char **argv)
{

   example=new Example2DWake();

   example->seed_particles(points, simulation_timestep);

   for (int i = 0; i < 1000; ++i)
      advance_simulation();

   std::ofstream particle_file;
   particle_file.open ("original.txt");
   write_matlab(particle_file, points, "particles");

   delete example;

   return 0;
}