#include <iostream>
#include <math.h>

using namespace std;

class VM{
	
	private:
		float vmNew;
		float core_i;
		float vmOld;
		float core_i0;
		float gamma_i;
		int i;

	public:
		VM(int id , float gamma){
			i = id;
			gamma_i = gamma;
		}

		int getID() {
			return i;
		}

		float getGamma() {
			return gamma_i;
		}
	
		float getVMNew(){
			return vmNew;
		}
	
		float getVMOld(){
			return vmOld;
		}

		float getCore() {
			return core_i;
		}

		float getCore_0() {
			return core_i0;
		}
	
		void setVMNew(float vm){
			vmNew=vm;
		}

		void setVMNewUsingCore() {
			vmNew = core_i/gamma_i;
			vmNew = ceil(vmNew);
		}
	
		void setVMOld(float vm){
			vmOld=vm;
		}

		void setCore(float core) {
			 core_i=core;
		}

		void setCore_0(float core) {
			core_i0=core;
		}
	
};
