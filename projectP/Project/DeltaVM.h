
#include <iostream>


class DeltaVM {

private:

	int deltaVM_i;
	int VM_i;
	int deltaVM_j;
	int VM_j;
	float Weight;


public:
	DeltaVM(){}
	
	DeltaVM(int deltaVM_1, int deltaVM_2, int i, int j, float weight) {
		deltaVM_i = deltaVM_1;
		deltaVM_j = deltaVM_2;
		VM_i = i;
		VM_j = j;
		Weight = weight;
	}

	int getdeltaVM_iToADD(){
		return deltaVM_i;
	}

	int getVM_i() {
		return VM_i;
	}

	int getdeltaVM_jToREMOVE() {
		return deltaVM_j;
	}

	int getVM_j() {
		return VM_j;
	}

	float getWeight(){
		return Weight;
	}

};
