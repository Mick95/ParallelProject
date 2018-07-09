// Project.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "Simulator.h"
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
std::vector<Simulator> t;
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


void calculateDenominator(vector<Application> app) {


	#pragma omp parallel for reduction (+: denominator)
	for ( int i = 0; i < app.size(); i++) {

		if ( app[i].i == 1) {
			firstElement = ComputeProduct(1, app[i].Gamma_i,  app[i].chi_c, app[i].chi_0, app[i].w_i);
		}
		else {
			ComputeProduct  c(app[i].i, app[i].Gamma_i, app[i].chi_c, app[i].chi_0 , app[i].w_i);
			product.push_back(c);
			denominator = denominator + sqrt(product.back().getWX());
		}
	}
}

VM calculateVM(ComputeProduct element, float den, float firstElem) {

	VM vmNew(element.getI(), element.getGamma(), element.getChi_c(), element.getChi_0() );
	
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

float weightedTardiness() {

}

int findRiscontro(int i, int j , int min) {

	int core_i = 0;

	#pragma omp parallel for
	for (int core_j = 1; core_j < (int) vectorVm[j].getVM(); core_j++) {
		
		while (core_i <= core_j && core_i*vectorVm[i].getGamma() <= core_j * vectorVm[j].getGamma() && min >= core_i ) {

			if (core_i*vectorVm[i].getGamma() == core_j * vectorVm[j].getGamma()) {

				if (weightedTardiness() < 0)

					newElement = DeltaVM(core_i, core_j, i, j, weightedTardiness());

		}

			core_i = 0;
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
