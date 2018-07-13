#pragma once
class ComputeProduct
{
private:
	int i;
	float wx_i;
	float chi_i;
	float chi_0;
	float gamma_i;
	float D_i;
	float w_i;

public:
	ComputeProduct();
	ComputeProduct(int id, float gamma, float chi_i, float chi_0, float w_i, float d_i);

	float getWX();

	float getGamma();

	float getDeadLine();

	float getWeight();

	float getChi_c();

	float getChi_0();

	int getI();

};

