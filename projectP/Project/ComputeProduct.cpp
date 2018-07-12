#include "stdafx.h"
#include "ComputeProduct.h"


ComputeProduct::ComputeProduct()
{
}


ComputeProduct::ComputeProduct(int id, float gamma, float chi_i, float chi_0, float w_i, float d_i) {
	i = id;
	wx_i = chi_i * w_i;
	gamma_i = gamma;
	D_i = d_i;
	this->chi_i = chi_i;
	this->chi_0 = chi_0;
}

float ComputeProduct::getWX() {
	return wx_i;
}

float ComputeProduct::getGamma() {
	return gamma_i;
}

float ComputeProduct::getDeadLine() {
	return D_i;
}

float ComputeProduct::getChi_c() {
	return chi_i;
}

float ComputeProduct::getChi_0() {
	return chi_0;
}

int ComputeProduct::getI() {
	return i;
}

