#include "stdafx.h"
#include "DeltaVM.h"


DeltaVM::DeltaVM()
{
}


DeltaVM::DeltaVM(int deltaVM_1, int deltaVM_2, VM i, VM j, float weight) {
	deltaVM_i = deltaVM_1;
	deltaVM_j = deltaVM_2;
	VM_i = i;
	VM_j = j;
	Weight = weight;
}

int DeltaVM::getdeltaVM_iToADD() {
	return deltaVM_i;
}

VM DeltaVM::getVM_i() {
	return VM_i;
}

int DeltaVM::getdeltaVM_jToREMOVE() {
	return deltaVM_j;
}

VM DeltaVM::getVM_j() {
	return VM_j;
}

float DeltaVM::getWeight() {
	return Weight;
}

void DeltaVM::setdeltaVM_iToADD(int elem) {
	deltaVM_i=elem;
}

void DeltaVM::setVM_i(VM vm) {
	VM_i=vm;
}

void DeltaVM::setdeltaVM_jToREMOVE(int elem) {
	 deltaVM_j=elem;
}

void DeltaVM::setVM_j(VM vm) {
	VM_j=vm;
}

void DeltaVM::setWeight(float weight) {
	 Weight=weight;
}



