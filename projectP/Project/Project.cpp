// Project.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
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
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

float denominator = 0;
ComputeProduct firstElement;
std::vector<Application> application;
std::vector<DeltaVM> L;
std::vector<DeltaVM> m;
std::vector<ComputeProduct> product;
std::vector<VM> vectorVm;
int n;
int maxIteration = 200;

//create The result File
void createFinalResult() {

	std::ofstream outfile("result.csv");

	outfile << "List of Virtual Machine for each application";

	for (int count = 0; count < vectorVm.size(); count++) {

		outfile << "\nID: " << vectorVm[count].getID();
		outfile << "\nVM: " << vectorVm[count].getVM();
		outfile << "\nCore: " << vectorVm[count].getCore();

	}

	outfile.close();

}


//modify the old vector of virtual machine after the movement of some core 
void modifyVectorVM() {



#pragma omp parallel for
	for (int count1 = 0; count1 < vectorVm.size(); count1++) {

		if (m[0].getVM_i().getID() == vectorVm[count1].getID()) {

			//define t1 and t2
			vectorVm[count1].setT2(vectorVm[count1].getT1());
			vectorVm[count1].setT1(m[0].getTi());
			//define VMOld and VM
			vectorVm[count1].setVMLastIteration(vectorVm[count1].getVM());
			vectorVm[count1].setVM(m[0].getdeltaVM_iToADD() + m[0].getVM_i().getVM());
			//define the core for each VM
			vectorVm[count1].setCoreAfterSimulator(vectorVm[count1].getVM());

		}

		if (m[0].getVM_j().getID() == vectorVm[count1].getID()) {

			//define t1 and t2
			vectorVm[count1].setT2(vectorVm[count1].getT1());
			vectorVm[count1].setT1(m[0].getTj());
			//define VMOld and VM
			vectorVm[count1].setVMLastIteration(vectorVm[count1].getVM());
			vectorVm[count1].setVM(m[0].getVM_j().getVM() - m[0].getdeltaVM_jToREMOVE());
			//define the core for each VM
			vectorVm[count1].setCoreAfterSimulator(vectorVm[count1].getVM());

		}
	}

	m.clear();

}

//check the best move for this iteration
void checkBestSolution(DeltaVM newDelta) {

	float actualBest;
	float newElement;

	actualBest = m[0].getTi() + m[0].getTj() - m[0].getVM_i().getDeadLine() - m[0].getVM_j().getDeadLine();
	newElement = newDelta.getTi() + newDelta.getTj() - newDelta.getVM_i().getDeadLine() - newDelta.getVM_j().getDeadLine();

	if (newElement < actualBest) {
		m.clear();
		m.push_back(newDelta);
	}

}


//opens the file generated by th simulator
float readFileFromSimulator() {

	float t1;
	t1 = 0;
	ifstream file;
	file.open("modifiedElement.csv");


	//check if file is open or exist
	if (file.is_open())
			file >> t1;

	file.close();
	remove("modifiedElement.csv");

	return t1;

}

//create the file to send to the simulator
void createFile() {

	float noise;
	float actualBest;
	srand((unsigned)time(NULL));
	noise = (rand() % 400 + 800);
	noise = (noise / 1000);

	//compute t1 for all the VM that are receiving more VM 
	for (int i = 0; i < L.size(); i++) {
		
		//cout << i << " "<<L[i].getVM_i().getID() << " " << L[i].getVM_j().getID() << endl;

		std::ofstream outfile("delta.csv");
		outfile << "Noise" << endl;
		outfile << noise << endl;
		outfile << "VMToAddi" << endl;
		outfile << L[i].getdeltaVM_iToADD() << endl;
		outfile << "Chi_0i" << endl;
		outfile << L[i].getVM_i().getChi_0() << endl;
		outfile << "Chi_ii" << endl;
		outfile << L[i].getVM_i().getChi_i() << endl;
		outfile << "VM_i" << endl;
		outfile << L[i].getVM_i().getVM() << endl;
		outfile << "gamma_i" << endl;
		outfile << L[i].getVM_i().getGamma() << endl;
		outfile.close();

		system("C:\\Users\\Windows\\Desktop\\GitHub\\ParallelProject\\projectP\\Project\\Simulator\\Debug\\Simulator.exe");
		L[i].setTi(readFileFromSimulator());


		remove("delta.csv");

	}

	//compute t1 for all the VM that are giving VM 
	for (int i = 0; i < L.size(); i++) {

		//cout << i << " " << L[i].getVM_i().getID() << " " << L[i].getVM_j().getID() << endl;

		std::ofstream outfile("delta.csv");
		outfile << "Noise" << endl;
		outfile << noise << endl;
		outfile << "VMToRemovej" << endl;
		outfile << -L[i].getdeltaVM_jToREMOVE() << endl;
		outfile << "chi_0j" << endl;
		outfile << L[i].getVM_j().getChi_0() << endl;
		outfile << "Chi_ij" << endl;
		outfile << L[i].getVM_j().getChi_i() << endl;
		outfile << "VM_j" << endl;
		outfile << L[i].getVM_j().getVM() << endl;
		outfile << "gamma_j" << endl;
		outfile << L[i].getVM_j().getGamma() << endl;
		outfile.close();

		system("C:\\Users\\Windows\\Desktop\\GitHub\\ParallelProject\\projectP\\Project\\Simulator\\Debug\\Simulator.exe");
		L[i].setTj(readFileFromSimulator());


		remove("delta.csv");

	}


	//find the best movement of VM
	for (int i = 0; i < L.size(); i++) {

		if (m.empty())
			m.push_back(L[i]);
		else
			checkBestSolution(L[i]);

		L.clear();
	}



	actualBest = m[0].getTi() + m[0].getTj() - m[0].getVM_i().getDeadLine() - m[0].getVM_j().getDeadLine();
	if (actualBest >= 0)
		m.clear();

}

//calculate the weight tardiness for 2 VM
float weightedTardiness(VM vm_i, VM vm_j, int i, int j) {

	float weight1;
	float weight2;

	//if I have done 2 iteration
	if (vm_i.getT1() != 0 && vm_i.getT2() != 0) {
	
		//compute a for i  
		vm_i.setA((vm_i.getT1() - vm_i.getT2()) / (vm_i.getVMLastIteration() - vm_i.getVM()));
		vm_i.setA(vm_i.getA() * vm_i.getVM()*vm_i.getVMLastIteration());
		//compure b for i
		vm_i.setB(vm_i.getT2() - vm_i.getA()/vm_i.getVMLastIteration());
		weight1=(vm_i.getA()/(vm_i.getVM() + i)) + vm_i.getB();

	}
	else {
		weight1 = (vm_i.getChi_i() / (vm_i.getVM() + i)) + vm_i.getChi_0();
	}
	
	//if I have done 2 iteration
	if (vm_j.getT1() != 0 && vm_j.getT2() != 0) {
		
		//compute a for j 
		vm_j.setA((vm_j.getT1() - vm_j.getT2()) / (vm_j.getVMLastIteration() - vm_j.getVM()));
		vm_j.setA(vm_j.getA() * vm_j.getVM()*vm_j.getVMLastIteration());
		//compute b for j
		vm_j.setB(vm_i.getT2() - vm_j.getA() / vm_j.getVMLastIteration());
		weight2 = (vm_j.getA() / (vm_j.getVM() - j)) + vm_j.getB();

	}
	else {
		weight2 = (vm_j.getChi_i() / (vm_j.getVM() - j)) + vm_j.getChi_0();
	}

	//weight of i
	weight1 = weight1 - vm_i.getDeadLine();
	weight1 = weight1 * ( vm_i.getWeight());

	//weight of j
	weight2 = weight2 - vm_j.getDeadLine();
	weight2 = weight2 * vm_j.getWeight();

	return (weight1 + weight2);
}

//for each couple search the minimum number of element that can be removed and added
void createElementForL(VM vm_i, VM vm_j) {

	int vmToADD_i;
	int vmToREMOVE_j = 1;
	bool elementFound = true;
	float weight;

	if (vm_i.getID() != vm_j.getID()) {

		//number of VM that i can remove from j
		while (vmToREMOVE_j < vm_j.getVM() && elementFound) {

			vmToADD_i = 0;

			// number of virtual machine that i can add to i
			while (vmToADD_i * vm_i.getGamma() <= vmToREMOVE_j * vm_j.getGamma() && elementFound) {

				//check condition on number of VM
				if (vmToADD_i * vm_i.getGamma() == vmToREMOVE_j * vm_j.getGamma() && (vm_j.getVM()-vmToREMOVE_j) > 0) {

					weight = weightedTardiness(vm_i, vm_j, vmToADD_i, vmToREMOVE_j);


					//possible new element to add to m
					if (weight < 0) {
						
#pragma omp critical
						L.push_back(DeltaVM(vmToADD_i, vmToREMOVE_j, vm_i, vm_j, weight));
						elementFound = false;
					
					}
				}	

				vmToADD_i++;

			}

			vmToREMOVE_j++;

		}
	}
}

//scan all couple of element to send it to createPossibleElementToAdd 
void computeSearch() {

#pragma omp parallel for 
	for (int vm_i = 0; vm_i < vectorVm.size(); vm_i++) {

			//find the element to change the value and create a list with the min number of element
#pragma omp parallel for 
		for (int vm_j = 0; vm_j < vectorVm.size(); vm_j++) {

			createElementForL(vectorVm[vm_i], vectorVm[vm_j]);
			

		}

	}

}

// check if i have enought core for all my VM
void checkFeasibilityVM() {

	float sum = n;
	float minimumWeight;
	int minID;

	//controll the total number of core
#pragma omp parallel for reduction (-: sum )
	for (int i = 0; i < vectorVm.size(); i++) {

		sum = sum - vectorVm[i].getCore();

	}

	while (sum < 0) {

		minimumWeight = 1000;
		minID = -1;

		//search for the element with the minimumWeight
#pragma omp parallel for  
		for (int i = 0; i < vectorVm.size(); i++) {

#pragma omp critical 
			if (minimumWeight > vectorVm[i].getWeight() && vectorVm[i].getCore() > 1) {
				
				minimumWeight = vectorVm[i].getWeight();
					minID = i;

			}


		}

		// remove n-i core from minID
		if (vectorVm[minID].getCore() + sum <= 0) {
			sum = sum + (vectorVm[minID].getCore()-1);
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

//calculate the number of VM for each application
VM calculateVM(ComputeProduct element, float den, float firstElem) {

	VM vmNew(element.getI(), element.getGamma(), element.getDeadLine(), element.getChi_c(), element.getChi_0(), element.getWeight());

	//for first object
	if (vmNew.getID() == 1) {
		vmNew.setCore(n / den);	
	}

	//for the other object
	else {
		vmNew.setCore(n*(sqrt(element.getWX() / firstElem) / den));	
	}

	vmNew.setCore(ceil(vmNew.getCore()));
	vmNew.setVM(vmNew.getCore() / element.getGamma());
	vmNew.setVM(ceil(vmNew.getVM()));
	
	return vmNew;
}

//create the vector of VM
void defineVectorVM() {
#pragma omp parallel for
	for (int i = 0; i < product.size(); i++) {
		VM vmNew = calculateVM(product[i], denominator, firstElement.getWX());
#pragma omp critical
		vectorVm.push_back(vmNew);

	}
}

// calculate the sum of the denominator of c
void computeDenominator(vector<Application> app) {


#pragma omp parallel for reduction (+: denominator)
	for (int i = 0; i < app.size(); i++) {

		//create the first element with id = to 1
		if (app[i].i == 1) {
			firstElement = ComputeProduct(1, app[i].Gamma_i, app[i].chi_c, app[i].chi_0, app[i].w_i, app[i].D_i);
		}

		// add a new element to the list of pruduct and define the denominator
		else {
			ComputeProduct  c(app[i].i, app[i].Gamma_i, app[i].chi_c, app[i].chi_0, app[i].w_i, app[i].D_i);
			product.push_back(c);
			denominator = denominator + sqrt(product.back().getWX());
		}
	}

	app.clear();

}

//read the application to use in this project
void readApplicationFile() {

	Application a;
	string as;
	
	std::ifstream readFile("application.csv");

	while (!readFile.eof()) {

		readFile >> as;
		readFile >> a.i;
		readFile >> as;
		readFile >> a.chi_0;
		readFile >> as;
		readFile >> a.chi_c;
		readFile >> as;
		readFile >> a.D_i;
		readFile >> as;
		readFile >> a.Gamma_i;
		readFile >> as;
		readFile >> a.w_i;
		if(!readFile.eof())
		application.push_back(a);

	}
}

int main() {

	int count = 0;
	bool check = true;
	n = 16;

	readApplicationFile();

	computeDenominator(application);
	denominator = 1 + (denominator / sqrt(firstElement.getWX()));


	vectorVm.push_back(calculateVM(firstElement, denominator, 1));
	defineVectorVM();
	checkFeasibilityVM();

	product.clear();

	while (check && count < maxIteration) {

		computeSearch();

		if (!L.empty()) {
			createFile();
		}
		else {
			check = false;
			count = 1000;
		}

		if ( m.empty() )
		{
			check = false;
		}
		else {
			modifyVectorVM();
			checkFeasibilityVM();
		}
		count++;
	}

	createFinalResult();
}
