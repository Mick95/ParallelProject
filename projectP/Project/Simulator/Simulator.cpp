// Simulator.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "VMSimulator.h"
#include <omp.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

VMSimulator fromFile;
float t1;


void createNewFile() {

	std::ofstream outfile("modifiedElement.csv");
	
	outfile << "t1:" << endl;
	outfile << t1 << endl;

	outfile.close();

}


void lundstromControll(float noise)  {

		//compute t1 and t2
		t1= fromFile.chi_ci / ((fromFile.vm_i + fromFile.deltaVM_i)*fromFile.gamma_i) + fromFile.chi_0i;
		t1= t1 * noise;		
		
}

int main() {

	ifstream file;
	file.open("delta.csv");
	float noise;
	string element;

	if (file.is_open()) {

		file >> element;
		file >> noise;
		
		file >> element;
		file >> fromFile.deltaVM_i;

		file >> element;
		file >> fromFile.chi_0i;

		file >> element;
		file >> fromFile.chi_ci;

		file >> element;
		file >> fromFile.vm_i;

		file >> element;
		file >> fromFile.gamma_i;

		file.close();

		lundstromControll(noise);

		createNewFile();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));


	}
}
