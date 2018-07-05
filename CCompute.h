#include <iostream>
using namespace std;

class CCompute{
	public:
		
	int i;
	float wx_i;
	float gamma_i;
	
	CCompute(int id, float gamma,float chi_i,float w_i){
		i=id;
		wx_i=chi_i*w_i;
		gamma_i=gamma;
	}
	
};
