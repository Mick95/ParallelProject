#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <list>
#include "Application.h"
#include "CCompute.h"
#include "VM.h"

using namespace std;

float denominator=0;
	float firstElementGamma;
	float firstElementWX;
	std::list<CCompute> product;
	std::list<VM> v;
	numberOfCore n;
	
int main(){
	
	
	
}

int calculateVi(list<Application> app){
	
	
	for(std::list<Application>::iterator it = app.begin(); it != app.end(); it++){
	
		if(it->i==1) {
			firstElementGamma=it->Gamma_i;
			firstElementWX=it->chi_c*it->w_i; 
		}
		else {
			product.push_back(CCompute(it->i, it->Gamma_i, it->chi_c, it->w_i));
			denominator = denominator + product.back().wx_i;
		}	
		
	}

	denominator = 1 + sqrt(denominator/firstElementWX);	
	
	
	v.push_back( VM(n*n, denominator, 1, firstElementGamma, 1));
	
	for(std::list<CCompute>::iterator it = product.begin(); it != product.end(); it++){
		
		v.push_back( VM(it->wx_i, denominator, firstElementWX, it->gamma_i, it->i));
	
	}
}
