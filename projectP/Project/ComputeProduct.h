#include <iostream>

class ComputeProduct{
	
	private:
		int i;
		float wx_i;
		float chi_i;
		float chi_0;
		float gamma_i;
		float D_i;
	public:
		ComputeProduct(){}

		ComputeProduct(int id, float gamma, float chi_i, float chi_0, float w_i, float d_i){
			i=id;
			wx_i=chi_i*w_i;
			gamma_i=gamma;
			D_i = d_i;
			this->chi_i = chi_i;
			this->chi_0 = chi_0;
		}
	
		float getWX(){
			return wx_i;
		}

		float getGamma(){
			return gamma_i;
		}

		float getDeadLine() {
			return D_i;
		}

		float getChi_c() {
			return chi_i;
		}

		float getChi_0() {
			return chi_0;
		}

		int getI(){
			return i;
		}
		
};
