
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iostream>
std::mt19937 rng;        
#if 0
void polynomial(float* ret, const float* const r_values, int num)
{
	//r*r*r*(10+r*(-15+r*6));


#if 1
	for (int i = 0; i < num; ++i)
	{
		float r = r_values[i];
		ret[i] = r*r*r*(10+r*(-15+r*6));
	}
#else
	for (int i = 0; i < num; ++i)
	{

		float r = r_values[i];
		float left =r*r;
		float right=r*6;
							
		left*=r;  
		right-=15;
			
		right*=r; 		
		
		ret[i] = left * ( 10 + right); 		
	}
#endif
}
#endif
float sum_polynomial (const float* const r_values, int num)
{
	const float* rr = static_cast<const float*>(__builtin_assume_aligned(r_values, 32));
	float sum = 0.f;
	const int loop_factor = 8;
	int num_loops = num / loop_factor;

	for (int i = 0; i < loop_factor; ++i) {
		for (int j = 0; j < num_loops; ++j)
		{
			float r = rr[j * loop_factor + i];
			//sum += r*r*r*(10+r*(-15+r*6));
			
			float left =r*r;
			float right=r*6;
								
			left*=r;  
			right-=15;
				
			right*=r; 		
			
			sum += left * ( 10 + right); 			
		}
	}	
	return sum;
}
int main(int argc, char* argv[])
{
	const int test_size = 4096 * 2;
	std::normal_distribution<float> dist(0.5f, 0.5f); 
	rng.seed(0);
	
	float* r_values = static_cast<float*>(_mm_malloc(sizeof(float) * test_size, 32));

	std::function<float()> rnd = std::bind(dist, rng);
	std::generate_n(r_values, test_size, rnd);

	double sum = 0.0f;
	//std::vector<float> ret(test_size);
	//std::vector<float> ret2(test_size);	

	for (int i = 0 ; i < 100000; ++i)
	{
		//polynomial(&ret[0], &r_values[0],test_size);
		//polynomial(&ret2[0], &ret[0],test_size);		
		
		//sum = std::accumulate(ret.begin(), ret.end(), sum);
		sum += sum_polynomial(&r_values[0],test_size);
		
    }
    std::cout << sum << std::endl;
}
