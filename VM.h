#include <iostream>
#include <math.h>

using namespace std;

class VM{
	public:
	
	float vm_i;
	float core_i;
	int i;
	
	VM(float wx_i, float den, float firstElem, float gamma_i, int id){
		core_i=sqrt(wx_i/firstElem)/den;
		vm_i=core_i/gamma_i;
		vm_i=ceil(vm_i);
		i=id;
	}
};
