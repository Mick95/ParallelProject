#include <iostream>

class ComputeProduct{
	
	private:
		int i;
		float wx_i;
		float a;
		float b;
		float gamma_i;
	public:
		ComputeProduct(){}

		ComputeProduct(int id, float gamma, float chi_i, float chi_0, float w_i){
			i=id;
			wx_i=chi_i*w_i;
			gamma_i=gamma;

			a = chi_i;
			b = chi_0;
		}
	
		float getWX(){
			return wx_i;
		}
		float getGamma(){
			return gamma_i;
		}
		float getChi_c() {
			return a;
		}

		float getChi_0() {
			return b;
		}

		int getI(){
			return i;
		}
		
};
