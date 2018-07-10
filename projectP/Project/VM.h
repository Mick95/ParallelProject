#include <iostream>

 class VM {
	
	private:
		float vm;
		float core_i;
		float gamma_i;
		float deadLine_i;
		float a=0;
		float b=0;
		float t1;
		float t2;
		float chi_i;
		float chi_0;
		int i;

	public:
		VM(int id , float gamma, float d_i, float chi_i, float chi_0){
			i = id;
			gamma_i = gamma;
			deadLine_i = d_i;
			this->chi_i = chi_i;
			this->chi_0 = chi_0;
			t1 = chi_i;
			t2 = chi_0;
		}

		int getID() {
			return i;
		}

		float getGamma() {
			return gamma_i;
		}
	
		float getDeadLine() {
			return deadLine_i;
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
	
		float getT1() {
			return t1;
		}

		float getT2() {
			return t2;
		}

		float getChi_i() {
			return chi_i;
		}

		float getChi_0() {
			return chi_0;
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

		void setT1(float t1) {
		this->t1 = t1;
		}

		void setT2(float t2) {
			this->t2 = t2;
		}
};
