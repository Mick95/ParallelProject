#pragma once
#include "VM.h"

class DeltaVM
{
private:
	int deltaVM_i;
	VM VM_i;
	int deltaVM_j;
	VM VM_j;
	float Weight;
	float t_i;
	float t_j;

public:
	DeltaVM();
	DeltaVM(int deltaVM_1, int deltaVM_2, VM i, VM j, float weight);

	int getdeltaVM_iToADD();

	VM getVM_i();

	int getdeltaVM_jToREMOVE();

	VM getVM_j();

	float getWeight();

	float getTi();

	float getTj();

	void setdeltaVM_iToADD(int elem);

	void setVM_i(VM vm);

	void setdeltaVM_jToREMOVE(int elem);

	void setVM_j(VM vm);

	void setWeight(float weight);

	void setTi(float ti);

	void setTj(float tj);

};


