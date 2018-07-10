// Project.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
//#include "Simulator.h"
#include "Application.h"
#include "ComputeProduct.h"
#include "DeltaVM.h"
#include "VM.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;

float denominator = 0;
ComputeProduct firstElement;
DeltaVM newElement;
//std::vector<Simulator> t;
std::vector<ComputeProduct> product;
std::vector<VM> vectorVm;
int n;


//void setSimulator(vector<Application> app) {
//	#pragma omp parallel for
//	for (int i = 0; i < app.size(); i++) {
//		Simulator simulator( app[i].chi_c , app[i].chi_0);
//		t.push_back(simulator);
//	}
//}

// calculate the sum of the denominator of c
void calculateDenominator(vector<Application> app) {


	#pragma omp parallel for reduction (+: denominator)
	for ( int i = 0; i < app.size(); i++) {

		if ( app[i].i == 1) {
			firstElement = ComputeProduct(1, app[i].Gamma_i,  app[i].chi_c, app[i].chi_0, app[i].w_i, app[i].D_i);
		}
		else {
			ComputeProduct  c(app[i].i, app[i].Gamma_i, app[i].chi_c, app[i].chi_0 , app[i].w_i, app[i].D_i);
			product.push_back(c);
			denominator = denominator + sqrt(product.back().getWX());
		}
	}
}

//calculate the number pf VM for each application
VM calculateVM(ComputeProduct element, float den, float firstElem) {

	VM vmNew(element.getI(), element.getGamma(), element.getDeadLine(), element.getChi_c(), element.getChi_0() );
	
	if (vmNew.getID() == 1) {
		vmNew.setCore( n / den);
	}
	else {
		vmNew.setCore(sqrt(element.getWX() / firstElem) / den);
	}

	vmNew.setVM(vmNew.getCore() / element.getGamma());
	vmNew.setVM(ceil(vmNew.getVM()));

	return vmNew;
}

void defineVectorVM() {

	#pragma omp parallel for
	for (int i = 0; i < product.size(); i++) {

		vectorVm.push_back(calculateVM(product[i], denominator, firstElement.getWX()));
	
	}
}


void checkFeasibilityVM() {
	
	float sum = n;
	float minimum;
	int minID;

	#pragma omp parallel for reduction (-: sum )
	for (int i = 0; i < vectorVm.size(); i++) {

		sum = sum - vectorVm[i].getCore();

	}

	while (sum < 0) {

		#pragma omp parallel for 
		for (int i = 0; i < vectorVm.size(); i++) {

			#pragma omp critical 
			if (minimum < vectorVm[i].getGamma() && vectorVm[i].getCore() > 1) {

				minID = i;

				}


		}

		if (vectorVm[minID].getCore() + sum < 0) {
			sum = sum + vectorVm[minID].getCore() - 1 ;
			vectorVm[minID].setCore(1);
		}

		else {
			vectorVm[minID].setCore(vectorVm[minID].getCore() + sum);
			sum = 0;
		}

		vectorVm[minID].setVM_UsingCore();

	}

}

float weightedTardiness(int i, int j, int vm_i, int vm_j ) {

	float weight1;
	float weight2;

	//compute a and b for i and j 
	// x2
	//vectorVm[i].setA(((vectorVm[i].getT1() - vectorVm[i].getT2()) / (vectorVm[i].getVM() - vectorVm[i].getVM_0()))*vectorVm[i].getVM()*vectorVm[i].getVM_0());
	//vectorVm[i].setB(vectorVm[i].getB()-vectorVm[i].getA()/vectorVm[i].getVM_0);

	//x2
	//weight1=vectorVm[i].getA()/(vectorVm[i].getVM() + vm_i) - vectorVm[i].getB();
	//weight1=weight1-vectorVM[i].getDeadLine();
	//weight1=weight1*vectorVM[i].getGamma();

	return weight1 + weight2;
}

int findRiscontro(int i, int j , int min) {

	int vm_i = 0;
	float weight;

	if (i != j) {

		for (int vm_j = 1; vm_j < (int)vectorVm[j].getVM(); vm_j++) {


			// core_i to add, core_j to remove
			// the second contraint is used to stop the cicle if there isn't any feasible condition
			// the third condition is used to stop the algorithm if the min number of core that i can remove from j to obtain a solution is less than core_i

			while (vm_i <= vm_j && vm_i*vectorVm[i].getGamma() <= vm_j * vectorVm[j].getGamma() && min >= vm_i) {

				if (vm_i*vectorVm[i].getGamma() == vm_j * vectorVm[j].getGamma()) {

					weight = weightedTardiness(i, j, vm_i, vm_j);

					if (weight < 0) {

						newElement = DeltaVM(vm_i, vm_j, i, j, weight);
						min = vm_j;
						return min;
					}

					vm_i++;
				}

				// if one of this condition is verified i have to go out from this cicle
				if (vm_i*vectorVm[i].getGamma() > vm_j * vectorVm[j].getGamma() || min < vm_i)
					return min;

				vm_i = 0;
			}
		}

	}
	return min;

}

void calcolo() {


	#pragma omp parallel for
	for (int vmToADD_i = 0; vmToADD_i < vectorVm.size(); vmToADD_i++) {
		int min = n;

		#pragma omp parallel for
		for (int vmToREMOVE_j = 0; vmToREMOVE_j < vectorVm.size(); vmToREMOVE_j++) {

			#pragma omp critical
			min=findRiscontro(vmToADD_i, vmToREMOVE_j, min);

		}

	}

}

int main() {

	n = omp_get_num_procs();
//	setSimulator(a);

	calculateDenominator(a);
	denominator = 1 + denominator / sqrt(firstElement.getWX());

	vectorVm.push_back(calculateVM(firstElement, denominator, 1));
	defineVectorVM();
	checkFeasibilityVM();

	std::vector<VM> vectorVmOld;
	

	vectorVmOld = vectorVm;

}
