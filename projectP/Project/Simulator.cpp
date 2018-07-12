#include "stdafx.h"
#include "DeltaVM.h"
#include "VM.h"
#include <omp.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

std::vector<DeltaVM> list;
std::vector<DeltaVM> fromFile;


void createNewFile() {

	std::ofstream outfile("modifiedElement.txt");
	for (int i = 0; i < list.size(); i++) {		
		outfile << list[i].getVM_i().getA();
		outfile << list[i].getVM_i().getB();
		outfile << list[i].getVM_i().getChi_0();
		outfile << list[i].getVM_i().getChi_i();
		outfile << list[i].getVM_i().getCondition();
		outfile << list[i].getVM_i().getCore();
		outfile << list[i].getVM_i().getDeadLine();
		outfile << list[i].getVM_i().getGamma();
		outfile << list[i].getVM_i().getID();
		outfile << list[i].getVM_i().getT1();
		outfile << list[i].getVM_i().getT2();
		outfile << list[i].getVM_i().getVM();
		outfile << list[i].getVM_i().getVMLastIteration();
		outfile << list[i].getVM_j().getA();
		outfile << list[i].getVM_j().getB();
		outfile << list[i].getVM_j().getChi_0();
		outfile << list[i].getVM_j().getChi_i();
		outfile << list[i].getVM_j().getCondition();
		outfile << list[i].getVM_j().getCore();
		outfile << list[i].getVM_j().getDeadLine();
		outfile << list[i].getVM_j().getGamma();
		outfile << list[i].getVM_j().getID();
		outfile << list[i].getVM_j().getT1();
		outfile << list[i].getVM_j().getT2();
		outfile << list[i].getVM_j().getVM();
		outfile << list[i].getVM_j().getVMLastIteration();
	}

	outfile.close();

}


void lundstromControll(){

	//to create a noise equal for all the computation 
	float noise;
	srand((unsigned)time(NULL));
	noise=(rand() % 400 + 800);
	noise=(noise/ 1000);

#pragma omp parallel for
	for(int i = 0; i < fromFile.size(); i++){

		float t;
		float t1;
		float t2;
		//compute t1 and t2
		t1 = fromFile[i].getVM_i().getChi_i() / (fromFile[i].getVM_i().getTemporaryCore(fromFile[i].getdeltaVM_iToADD())) + fromFile[i].getVM_i().getChi_0();
		t2 = fromFile[i].getVM_i().getChi_i() / (fromFile[i].getVM_j().getTemporaryCore(-fromFile[i].getdeltaVM_jToREMOVE())) + fromFile[i].getVM_j().getChi_0();
		t1 = t1 * noise;
		t2 = t2 * noise;

		t = t1 + t2;

		if (t < 0) {

			//define t1 and t2 for vm_i
			fromFile[i].getVM_i().setT2(fromFile[i].getVM_i().getT1());
			fromFile[i].getVM_i().setT1(t1);

			//define t1 and t2 for vm_j
			fromFile[i].getVM_j().setT2(fromFile[i].getVM_j().getT1());
			fromFile[i].getVM_j().setT1(t1);

			//define new core 
			fromFile[i].getVM_i().setCoreAfterSimulator(fromFile[i].getdeltaVM_iToADD());
			fromFile[i].getVM_j().setCoreAfterSimulator(-fromFile[i].getdeltaVM_jToREMOVE());

			// define new oldVM
			fromFile[i].getVM_i().setVMLastIteration(fromFile[i].getVM_i().getVM());
			fromFile[i].getVM_j().setVMLastIteration(fromFile[i].getVM_j().getVM());

			//define new vm
			fromFile[i].getVM_i().setVM(fromFile[i].getVM_i().getVM() + fromFile[i].getdeltaVM_iToADD());
			fromFile[i].getVM_j().setVM(fromFile[i].getVM_j().getVM() - fromFile[i].getdeltaVM_jToREMOVE());

			list.push_back(fromFile[i]);
			
		}

	}

}

int main() {

	ifstream file;
	file.open("delta.txt");

	DeltaVM newElement;
	
	int element1;
	float element2;
	bool element3;
	

	if (file.is_open())
	while (!file.eof()) {
		VM vm1 = VM();
		VM vm2 = VM();


		file >> element1;
		newElement.setdeltaVM_iToADD(element1);

		file >> element1;
		newElement.setdeltaVM_jToREMOVE(element1);

		file >> element2;
		newElement.setWeight(element2);

		file >> element2;
		vm1.setA(element2);

		file >> element2;
		vm1.setB(element2);

		file >> element2;
		vm1.setChi_0(element2);

		file >> element2;
		vm1.setChi_i(element2);

		file >> element3;
		vm1.setCondition(element3);
			
		file >> element2;
		vm1.setCore(element2);

		file >> element2;
		vm1.setDeadLine(element2);

		file >> element2;
		vm1.setGamma(element2);

		file >> element1;
		vm1.setID(element1);

		file >> element2;
		vm1.setT1(element2);

		file >> element2;
		vm1.setT2(element2);

		file >> element2;
		vm1.setVM(element2);

		file >> element2;
		vm1.setVMLastIteration(element2);

		file >> element2;
		vm2.setA(element2);

		file >> element2;
		vm2.setB(element2);

		file >> element2;
		vm2.setChi_0(element2);

		file >> element2;
		vm2.setChi_i(element2);

		file >> element3;
		vm2.setCondition(element3);

		file >> element2;
		vm2.setCore(element2);

		file >> element2;
		vm2.setDeadLine(element2);

		file >> element2;
		vm2.setGamma(element2);

		file >> element1;
		vm2.setID(element1);

		file >> element2;
		vm2.setT1(element2);

		file >> element2;
		vm2.setT2(element2);

		file >> element2;
		vm2.setVM(element2);

		file >> element2;
		vm2.setVMLastIteration(element2);

		newElement.setVM_i(vm1);
		newElement.setVM_j(vm2);

		fromFile.push_back(newElement);

	}

	file.close();

	remove("delta.txt");

	lundstromControll();

	createNewFile();

}

