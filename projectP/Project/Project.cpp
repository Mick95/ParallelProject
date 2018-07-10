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
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

float denominator = 0;
ComputeProduct firstElement;
std::vector<DeltaVM> newElement;
//std::vector<Simulator> t;
std::vector<DeltaVM> m;
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

		//create the first element with id = to 1
		if ( app[i].i == 1) {
			firstElement = ComputeProduct(1, app[i].Gamma_i,  app[i].chi_c, app[i].chi_0, app[i].w_i, app[i].D_i);
		}

		// add a new element to the list of pruduct and define the denominator
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
	
	//for first object
	if (vmNew.getID() == 1) {
		vmNew.setCore( n / den);
	}
	//for the other object
	else {
		vmNew.setCore(sqrt(element.getWX() / firstElem) / den);
	}

	vmNew.setVM(vmNew.getCore() / element.getGamma());
	vmNew.setVM(ceil(vmNew.getVM()));

	return vmNew;
}

//create the vector of VM
void defineVectorVM() {

#pragma omp parallel for
	for (int i = 0; i < product.size(); i++) {
		
		vectorVm.push_back(calculateVM(product[i], denominator, firstElement.getWX()));
	
	}
}

// check if i have enought core for all my VM
void checkFeasibilityVM() {
	
	float sum = n;
	float minimumGamma;
	int minID;

	//controll the total number of core
#pragma omp parallel for reduction (-: sum )
	for (int i = 0; i < vectorVm.size(); i++) {

		sum = sum - vectorVm[i].getCore();

	}

	minimumGamma = vectorVm[0].getGamma();
	minID = 0;

	while (sum < 0) {

		//search for the element with the minimumGamma
#pragma omp parallel for 
		for (int i = 0; i < vectorVm.size(); i++) {

#pragma omp critical 
			if (minimumGamma < vectorVm[i].getGamma() && vectorVm[i].getCore() > 1) {

				minID = i;

				}


		}

		// remove n-i core from minID
		if (vectorVm[minID].getCore() + sum < 0) {
			sum = sum + (vectorVm[minID].getCore() - 1) ;
			vectorVm[minID].setCore(1);
		}

		// remove [sum] core from minID
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

	if (vectorVm[i].getT1() != 0 && vectorVm[i].getT2() != 0) {
		
		//compute a for i  
		vectorVm[i].setA((vectorVm[i].getT1() - vectorVm[i].getT2()) / (vectorVm[i].getVMLastIteration() - vectorVm[i].getVM()));
		vectorVm[i].setA(vectorVm[i].getA() * vectorVm[i].getVM()*vectorVm[i].getVMLastIteration());
		//compure b for i
		vectorVm[i].setB(vectorVm[i].getT2() - vectorVm[i].getA()/vectorVm[i].getVMLastIteration());
		weight1=vectorVm[i].getA()/(vectorVm[i].getVM() + vm_i) - vectorVm[i].getB();

	}
	else {
		weight1 = vectorVm[i].getChi_i() / (vectorVm[i].getVM() + vm_i) - vectorVm[i].getChi_0();
	}

	if (vectorVm[j].getT1() != 0 && vectorVm[j].getT2() != 0) {

		//compute a for j 
		vectorVm[j].setA((vectorVm[j].getT1() - vectorVm[j].getT2()) / (vectorVm[j].getVMLastIteration() - vectorVm[j].getVM()));
		vectorVm[j].setA(vectorVm[j].getA() * vectorVm[j].getVM()*vectorVm[j].getVMLastIteration());
		//compute b for j
		vectorVm[j].setB(vectorVm[i].getT2() - vectorVm[j].getA() / vectorVm[j].getVMLastIteration());
		weight2 = vectorVm[j].getA() / (vectorVm[j].getVM() - vm_j) - vectorVm[j].getB();

	}
	else {
		weight2 = vectorVm[j].getChi_i() / (vectorVm[j].getVM() - vm_j) - vectorVm[j].getChi_0();
	}

	//weight of i
	weight1 = weight1 - vectorVm[i].getDeadLine();
	weight1 = weight1 * vectorVm[i].getGamma();
	//weight of j
	weight2 = weight2 - vectorVm[j].getDeadLine();
	weight2 = weight2 * vectorVm[j].getGamma();

	return weight1 + weight2;
}

void findRiscontro(int i, int j, std::vector<int> minList) {

	int vm_i;
	int vm_j = 1;
	bool boolean = true;
	float weight;

	if (i != j) {

		while (vm_j < (int)vectorVm[j].getVM() && boolean) {

			vm_i = 0;

			// core_i to add, core_j to remove
			// the second contraint is used to stop the cicle if there isn't any feasible condition

			while (vm_i <= vm_j && vm_i * vectorVm[i].getGamma() <= vm_j * vectorVm[j].getGamma() && boolean) {

				if (vm_i * vectorVm[i].getGamma() == vm_j * vectorVm[j].getGamma()) {

					weight = weightedTardiness(i, j, vm_i, vm_j);


					//possible new element to add to m
					if (weight < 0) {

#pragma omp critical
						newElement.push_back(DeltaVM(vm_i, vm_j, i, j, weight));
#pragma omp critical
						minList.push_back(vm_j);
						boolean = false;
					
					}
				}

				// if one of this condition is verified i have to go out from this cicle
				if (vm_i*vectorVm[i].getGamma() > vm_j * vectorVm[j].getGamma())

					boolean = false;

				vm_i++;
			}

			vm_j++;
		}
	}
}

void calcolo() {

#pragma omp parallel for 
	for (int vmToADD_i = 0; vmToADD_i < vectorVm.size(); vmToADD_i++) {

		if (!(vectorVm[vmToADD_i].getCondition())) {

			int minVMToRemove;
			std::vector<int> minList;


			//find the element to move the value and create a list with the min number of element
#pragma omp parallel for 
			for (int vmToREMOVE_j = 0; vmToREMOVE_j < vectorVm.size(); vmToREMOVE_j++) {

				if (!(!vectorVm[vmToREMOVE_j].getCondition())) {

					findRiscontro(vmToADD_i, vmToREMOVE_j, minList);

				}
			}

			//find the minimum number of VM removed
			std::vector<int>::iterator result = std::min_element(std::begin(minList), std::end(minList));
			minVMToRemove = minList[std::distance(std::begin(minList), result)];
			minList.clear();

			//find wich element has the min value to remove
			std::vector<DeltaVM> elementWithMinimumRemove;

#pragma omp parallel for
			for (int count = 0; count < newElement.size(); count++) {

				if (newElement[count].getdeltaVM_jToREMOVE() == minVMToRemove) {

#pragma omp critical
					elementWithMinimumRemove.push_back(newElement[count]);

				}
			}

			//find the min element with the value to add
			newElement.clear();
			if (!(elementWithMinimumRemove.empty())) {
				newElement.push_back(elementWithMinimumRemove[0]);
				for (int count = 1; count < elementWithMinimumRemove.size(); count++) {
					if (newElement[0].getdeltaVM_iToADD() > elementWithMinimumRemove[count].getdeltaVM_iToADD()) {
						newElement.clear();
						newElement.push_back(elementWithMinimumRemove[count]);
					}
				}

				m.push_back(newElement[0]);

			}
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
