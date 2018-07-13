#pragma once
class VM
{

private:
	float vm;
	float vmOld;
	float core_i;
	float gamma_i;
	float deadLine_i;
	float weight_i;
	float a = 0;
	float b = 0;
	float t1;
	float t2;
	float chi_i;
	float chi_0;
	bool optimizationDone = false;
	int i;

public:
	VM();
	VM(int id, float gamma, float d_i, float chi_i, float chi_0, float weight);

	int getID();

	float getGamma();

	float getDeadLine();

	float getVM();

	float getVMLastIteration();

	float getCore();

	float getWeight();

	float getA();

	float getB();

	float getT1();

	float getT2();

	float getChi_i();

	float getChi_0();

	bool getCondition();
	
	void setID(int id);

	void setVM(float vm);

	void setVM_UsingCore();

	void setVMLastIteration(float num);

	void setCoreAfterSimulator(float core);

	void setCore(float core);

	void setA(float t1);

	void setB(float t2);

	void setT1(float t1);

	void setT2(float t2);

	void setChi_0(float chi_0);

	void setChi_i(float chi_i);

	void setCondition(bool cond);
};

