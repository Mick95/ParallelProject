#include <iostream>

class Simulator{
	
	private:
		float a;
		float b;
	
	public:
		Simulator(float a1,float b1){
			a=a1;
			b=b1;
		}
	
		float getA(){
			return a;
		}
		
		float getB(){
			return b;
		}
	
		void setA(float a1){
			a=a1;
		}
	
		void setB(float b1){
			b=b1;
		}
};
