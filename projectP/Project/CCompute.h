#include <iostream>
using namespace std;

class CCompute{
	
	private:
		int i;
		float wx_i;
		float gamma_i;
	public:
		CCompute(int id, float gamma,float chi_i,float w_i){
			i=id;
			wx_i=chi_i*w_i;
			gamma_i=gamma;
		}
	
		float getWX(){
			return wx_i;
		}
		float getGamma(){
			return gamma_i;
		}
		int getI(){
			return i;
		}
		
};
