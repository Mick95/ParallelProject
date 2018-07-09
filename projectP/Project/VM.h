#include <iostream>
#include<math.h>

class VM{
	
	private:
		float vm;
		float core_i;
		float gamma_i;
		float a;
		float b;
		int i;

	public:
		VM(int id , float gamma, float chi_i, float chi_0){
			i = id;
			gamma_i = gamma;
			a = chi_i;
			b = chi_0;
		}

		int getID() {
			return i;
		}

		float getGamma() {
			return gamma_i;
		}
	
		float getVM(){
			return vm;
		}

		float getCore() {
			return core_i;
		}

		float getA() {
			return a;
		}

		float getB() {
			return b;
		}
	
		void setVM(float vm){
			vm=vm;
		}

		void setVM_UsingCore() {
			vm = core_i/gamma_i;
			vm = ceil(vm);
		}

		void setCore(float core) {
			 core_i=core;
		}

		void setA(float t1) {
			a = t1;
		}

		void setB(float t2) {
			b = t2;
		}
};
