
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
std::mt19937 rng;        

static void polynomial(std::vector<float> & ret, const std::vector<float> & r_values)
{
	//r*r*r*(10+r*(-15+r*6));

	const int loop_factor = 8;
	int iter = r_values.size() / loop_factor;
	for (int i = 0; i < iter; ++i)
	{
		float r[loop_factor];
		float tmp[loop_factor];
		float rv[loop_factor];
		for (int j = 0; j < loop_factor; ++j)
			r[j] = r_values[i*loop_factor + j];
		
		for (int j = 0; j < loop_factor; ++j)
			rv[j]=r[j]*r[j];			

		for (int j = 0; j < loop_factor; ++j)
			tmp[j]=r[j]*6;			

		for (int j = 0; j < loop_factor; ++j)
			rv[j]*=r[j];			

		for (int j = 0; j < loop_factor; ++j)
			tmp[j]-=15;			

		for (int j = 0; j < loop_factor; ++j)
			tmp[j]*=r[j];		
		
		for (int j = 0; j < loop_factor; ++j)
			ret[i*loop_factor+j]= rv[j] * (10 + tmp[j]);			
	}	

   
}

int main(int argc, char* argv[])
{
	const int test_size = 4096 * 2;
	std::normal_distribution<float> dist(0.5f, 0.5f); 
	rng.seed(0);
	std::vector<float> r_values(test_size);
	std::function<float()> rnd = std::bind(dist, rng);
	std::generate_n(r_values.begin(), test_size, rnd);

	double sum = 0.0f;
	std::vector<float> ret(test_size);
	std::vector<float> ret2(test_size);	
	std::vector<float> tmp(test_size);
	for (int i = 0 ; i < 100000; ++i)
	{
		polynomial(ret, r_values);
		polynomial(ret2, ret);		
		
		sum = std::accumulate(ret2.begin(), ret2.end(), sum);
		
    }
    std::cout << sum << std::endl;
}
