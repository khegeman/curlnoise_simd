


#include <iostream>
#include <fstream>


#include "example_2dwake.h"


float simulation_timestep=0.04;


std::vector<Vec2f> points;

CurlNoise2 *example=0;

void advance_simulation()
{
   static int pass = 0;
   pass++;

   Vec4f midx;   
   for(unsigned int i=0; i<points.size()/2; i++){
      // almost RK2 (not updating t halfway through time step, so it's technically only 1st order accurate)
      Vec4f two_points = Vec4f(points[i*2][0],points[i*2][1],points[i*2+1][0],points[i*2+1][1]);
      Vec4f v1 = example->get_velocity(two_points);
      midx=two_points+0.5f*simulation_timestep*v1;
      Vec4f vel4 = example->get_velocity(midx);
      points[i*2]+=simulation_timestep*Vec2f(vel4[0],vel4[1]);
      points[i*2+1]+=simulation_timestep*Vec2f(vel4[2],vel4[3]);      
   }

   if (points.size() % 2) {
      unsigned int i = points.size() - 1;
      Vec4f two_points = Vec4f(points[i][0],points[i][1],0.,0.);
      Vec4f v1 = example->get_velocity(two_points);
      midx=two_points+0.5f*simulation_timestep*v1;
      Vec4f vel4 = example->get_velocity(midx);
      points[i]+=simulation_timestep*Vec2f(vel4[0],vel4[1]);
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
   particle_file.open ("particles.txt");
   write_matlab(particle_file, points, "particles");

   delete example;

   return 0;
}